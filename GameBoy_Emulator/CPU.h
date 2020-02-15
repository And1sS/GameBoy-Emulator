#pragma once

#include <cstdint>

#include "Memory.h"

#define Z_BIT 7
#define N_BIT 6
#define H_BIT 5
#define C_BIT 4

#define B_REG 0
#define C_REG 1
#define D_REG 2
#define E_REG 3
#define H_REG 4
#define L_REG 5
#define A_REG 6
#define F_REG 7

class CPU
{
private:
	uint8_t regs[8]; // BC, DE, HL, AF
	uint8_t& F = regs[F_REG]; // Flag Register
	uint16_t SP; // Stack Pointer
	uint16_t PC; // Program Counter

	unsigned int clock_cycle = 0;

	Memory* mem;

	inline void set_z_bit();
	inline void reset_z_bit();

	inline void set_n_bit();
	inline void reset_n_bit();
	
	inline void set_h_bit();
	inline void reset_h_bit();
	
	inline uint8_t get_c_bit();
	inline void set_c_bit();
	inline void reset_c_bit();

	uint8_t add_with_carry(uint8_t a, uint8_t b, bool use_carry);

public:
	void execute_instruction();
};

inline void CPU::set_z_bit()
{
	F |= 1 << Z_BIT;
}

inline void CPU::reset_z_bit()
{
	F &= ~(1 << Z_BIT);
}

inline void CPU::set_h_bit()
{
	F |= 1 << H_BIT;
}

inline void CPU::reset_h_bit()
{
	F &= ~(1 << H_BIT);
}

inline void CPU::set_n_bit()
{
	F |= 1 << N_BIT;
}

inline void CPU::reset_n_bit()
{
	F &= ~(1 << N_BIT);
}

inline uint8_t CPU::get_c_bit()
{
	return (F & (1 << C_BIT)) >> C_BIT;
}

inline void CPU::set_c_bit()
{
	F |= 1 << C_BIT;
}

inline void CPU::reset_c_bit()
{
	F &= ~(1 << C_BIT);
}

uint8_t CPU::add_with_carry(uint8_t a, uint8_t b, bool use_carry)
{
	uint16_t a16 = a;
	uint16_t b16 = b;

	uint16_t res = a16 + b16;
	if (use_carry) 
		res += get_c_bit();

	if (res > 0xFF)
		set_c_bit();
	else
		reset_c_bit();

	uint8_t tmp = (a16 & 0x0F) + (b16 & 0x0F);
	if (use_carry)
		tmp += get_c_bit();

	if (tmp > 0x0F)
		set_h_bit();
	else
		reset_h_bit();

	res &= 0xFF;

	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	reset_n_bit();

	return res;
}