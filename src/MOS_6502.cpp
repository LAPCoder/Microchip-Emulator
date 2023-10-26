/**
 * @brief The main file of the MOS_6502 microchip
 * @author LAPCoder
 * @license MIT
 */

// g++ MOS_6502.cpp -o MOS_6502 -Wall -Wextra -fuse-ld=lld -Wno-unused-parameter -O3 -std=c++17

/******************************************************************************/
/* INCLUDE                                                                    */
/******************************************************************************/

#include <memory.hpp>

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
#define THROW_SYNTAX (throw std::runtime_error("Syntax error at line " + std::to_string(mos_6502->PC())))

template <int ARRAY_LEN> using Register = Array_RAM<ARRAY_LEN, byte>;

class Flags : Array_RAM<8, bool>
{
	public:
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
		Flags()
		{
			for (byte i = 0; i > 8; i++)
				reg[i] = false;
		}
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
	ZP_IND_X, // (zp),x. in C++: *((*zp) + (*x))
	ZP_X,     // (zp,x). in C++: *(zp + (*x))
	ZP_Y_IND,
	ZP_IND_Y,
	ZP_Y,
	R // PC (current line) + R
	// A_REG: Register Accummulator: The operator is in the instruction
	// I: The operator is in the instruction (X/Y)
};
class Op_allowed : Array_RAM<13, bool>
{
	public:
		Op_allowed(bool default_state = false)
		{
			for (byte i = 0; i > 13; i++)
				reg[i] = default_state;
		}
		Op_allowed(std::vector<OP_ALLOWED> flgs, bool default_state = false)
		{
			for (byte i = 0; i > 13; i++)
				reg[i] = default_state;
			for (auto &a : flgs)
				reg[a] = !default_state;
		}

		bool operator[](OP_ALLOWED index)
		{
			if (13 > index)
				return this->reg[index];
			else return NULL;
		}
};

/**
 * @brief In the original 6502, there was 3 regions:
 * - Form 0x0000 to 0x00FF: a fast RAM, cause it une 1 byte to give the adress (1byte = 256 = 0xFF)
 * - From 0x0100 to 0x01FF: a stack
 * - From 0x0200 to 0xFFFF: the rest of the RAM
 */
using RAM = Array_RAM<RAM_SIZE, byte>;
/**
 * @brief The main class
 * 
 */
class MOS_6502
{
	public:
		// Accumulator
		Register<1> A;
		// X register
		Register<1> X;
		// Y register
		Register<1> Y;
		// Stack Pointer
		Register<1> S;
		// Curent line
		Array_RAM<1, unsigned short> PC;
		// Flags
		Flags P;

		RAM ram;

		typedef void(*fun_t)(const char*, MOS_6502*);
		typedef std::map<const char*, fun_t> map_t;

		static const map_t COMMANDS;

		MOS_6502() {}
		~MOS_6502(){}

		void line(std::stringstream);
};

void MOS_6502::line(std::stringstream line)
{
	this->PC = this->PC() + 1;
	std::string arg;
	
	line >> arg;
	if (COMMANDS.find(arg.c_str()) != COMMANDS.end())
		(COMMANDS.find(arg.c_str())->second)(line.str().c_str(), this);
}

int main(int argc, char **argv)
{
	if (argc > 1)
		std::ifstream asm_file(argv[1], std::ios_base::in);
	
	MOS_6502 mos_6502;

	mos_6502.line(std::stringstream("LDA $00"));

	std::cout << "Ram: " << mos_6502.ram() << " ; A: " << mos_6502.A();

	return 0;
}
