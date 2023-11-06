/**
 * @brief Provide a CSV file reader
 * @authors @sastanin (Stack Overflow), LAPCoder
 * @license MIT
 */

#ifndef CSV_READER_HPP
#define CSV_READER_HPP

#include <istream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>

enum class CSVState {
	UnquotedField,
	QuotedField,
	QuotedQuote
};

std::map<std::pair<unsigned,unsigned>, std::string> readCSVRow(
	const std::string &row,
	const unsigned &row_id)
{
	CSVState state = CSVState::UnquotedField;
	std::map<std::pair<unsigned,unsigned>, std::string> fields;
	size_t i = 0; // index of the current field
	for (char c : row) {
		switch (state) {
			case CSVState::UnquotedField:
				switch (c) {
					case ',': // end of field
							fields[std::make_pair(row_id,++i)] = "";
							break;
					case '"': state = CSVState::QuotedField;
							break;
					default:  fields[std::make_pair(row_id,i)].push_back(c);
							break; }
				break;
			case CSVState::QuotedField:
				switch (c) {
					case '"': state = CSVState::QuotedQuote;
							break;
					default:  fields[std::make_pair(row_id,i)].push_back(c);
							break; }
				break;
			case CSVState::QuotedQuote:
				switch (c) {
					case ',': // , after closing quote
							fields[std::make_pair(row_id,++i)] = "";
							state = CSVState::UnquotedField;
							break;
					case '"': // "" -> "
							fields[std::make_pair(row_id,i)].push_back('"');
							state = CSVState::QuotedField;
							break;
					default:  // end of quote
							state = CSVState::UnquotedField;
							break; }
				break;
		}
	}
	return fields;
}

/// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
std::unordered_map<std::string, std::pair<unsigned,unsigned>> readCSV(std::istream &in) {
	std::unordered_map<std::string, std::pair<unsigned,unsigned>> table;
	std::string row;
	unsigned i = 0;
	while (!in.eof()) {
		std::getline(in, row);
		if (in.bad() || in.fail()) {
			break;
		}
		auto fields = readCSVRow(row, i);
		for (std::map<std::pair<unsigned,unsigned>,std::string>::iterator j =
			fields.begin(); j != fields.end(); ++j)
			table[j->second] = j->first;
		i++;
	}
	return table;
}

#endif // CSV_READER_HPP
