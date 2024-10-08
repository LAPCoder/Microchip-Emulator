/**
 * @brief The main file of the MOS_6502 microchip
 * @author LAPCoder
 * @license MIT
 */

// g++ src/MOS_6502.cpp -o MOS_6502 -Wall -Wextra -fuse-ld=lld -Wshadow -g -fsanitize=address,undefined -O3 -std=c++17

/******************************************************************************/
/* INCLUDE                                                                    */
/******************************************************************************/

#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <numeric>
#include <sstream>
#include <map>
#include <functional>
#include <algorithm>
#include <optional>
#include <type_traits>
#include <stdexcept>
#include <vector>

/******************************************************************************/
/* CLASS                                                                      */
/******************************************************************************/

#define RAM_SIZE 0xFFFF // Number of cases in the RAM (x sizeof byte)
#define THROW_SYNTAX (throw std::runtime_error("Syntax error at line " + std::to_string(__LINE__/*this->PC()*/)))

typedef unsigned char byte;

enum FLAGS
{
	C, // Carry
	Z, // Zero
	I, // Interrupt Disable
	D, // Decimal
	B, // Break
	_, // Unused
	V, // Overflow
	N  // Negative
};

enum OP_ALLOWED
{
	A, // Absolute address. Like a pointer
	A_IND, // Only for JMP. Like a pointer to a pointer (in C++: int**)
	A_X, // Absolute address + number stored in X (in C++: *(A+(*X)) )
	A_Y,
	S, // #. Use the value for the computation (in C++: int)
	ZP, // $xy in Zero Page: $00xy. Like A (A is 4-number)
	ZP_X_IND, // (zp,x). in C++: *(*(zp + (*x)))
	ZP_X,     // (zp,x). in C++: *(zp + (*x))
	ZP_IND_Y, // (zp),y. in C++: *((*zp) + (*y))
	ZP_Y,
	R = ZP // PC (current line) + number
	// A_REG: Register Accummulator: The operator is in the instruction
	// I: The operator is in the instruction (X/Y)
};

/**
 * @brief The main class
 * 
 */
class MOS_6502
{
	public:
		byte ram[0xFFFF] = {0};

		typedef void(*fun_t)(const char*, MOS_6502*);
		typedef std::map<const char*, fun_t> map_t;

		static const map_t COMMANDS;

		 MOS_6502() {}
		~MOS_6502() {}

		void execute(std::ifstream*);
		bool check_validity(std::ifstream*);
		OP_ALLOWED findArgType(std::string&);
};

OP_ALLOWED MOS_6502::findArgType(std::string &in)
{
	if (in.find(',')) // Case of all types contains X/Y registers
	{
		if (in[0] == '(')
		{
			if (*in.end() != ')')
			{
				if (*in.end() == 'Y')
					return ZP_IND_Y;
			}
			else
			{
				if (*(in.end()-1) == 'X')
					return ZP_X_IND;
			}
		}
		else
		{
			if (isdigit(in[3]))
				switch (*in.end())
				{
					case 'X': return A_X;
					case 'Y': return A_Y;
					default: THROW_SYNTAX;
				}
			else if (in[3] == ',')
				switch (*in.end())
				{
					case 'X': return ZP_X;
					case 'Y': return ZP_Y;
					default: THROW_SYNTAX;
				}
			else THROW_SYNTAX;
		}
	}
	else
		switch (in.length())
		{
			case 3: return ZP;
			// Or return R;
			case 5:	return OP_ALLOWED::A;
			case 7:	return A_IND;
			case 4:	return OP_ALLOWED::S;
			default: THROW_SYNTAX;
		}
	THROW_SYNTAX;
}

bool MOS_6502::check_validity(std::ifstream *file)
{
	// Algo
	//...
	return true;
}

void MOS_6502::execute(std::ifstream *file)
{
	for (std::string line; std::getline(*file, line);) 
	{
		
	}
	

	return;
}

int main(int argc, char **argv)
{
	std::ifstream *asm_file;
	if (argc > 1)
		// put the file named in argv[1] in asm_file
		asm_file = new std::ifstream(argv[1], std::ios_base::in);
	else exit(-1);
	
	MOS_6502 mos_6502;

	if (mos_6502.check_validity(asm_file))
		mos_6502.execute(asm_file);

	delete asm_file;

	return 0;
}
