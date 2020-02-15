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


// TODO: IMPLEMENT CONSTRUCTOR


class CPU
{
private:
	uint8_t regs[8]; // BC, DE, HL, AF
	uint8_t& F = regs[F_REG]; // Flag Register
	uint16_t SP; // Stack Pointer
	uint16_t PC; // Program Counter

	unsigned int clock_cycle = 0;

	Memory* mem;

	inline uint8_t read_instr();
	inline void set_z_bit();
	inline void reset_z_bit();

	inline void set_n_bit();
	inline void reset_n_bit();
	
	inline void set_h_bit();
	inline void reset_h_bit();
	
	inline uint8_t get_c_bit();
	inline void set_c_bit();
	inline void reset_c_bit();

	inline uint8_t get_HL();
	inline void set_HL(uint8_t a);


	uint8_t add_instr(uint8_t a, uint8_t b, bool use_carry);
	uint8_t sub_instr(uint8_t a, uint8_t b, bool use_carry);
	uint8_t and_instr(uint8_t a, uint8_t b); 
	uint8_t or_instr(uint8_t a, uint8_t b);
	uint8_t xor_instr(uint8_t a, uint8_t b);
	void cp_instr(uint8_t a, uint8_t b);
	uint8_t inc_instr(uint8_t a);
	uint8_t dec_instr(uint8_t a);

public:
	void execute_instruction();
};

inline uint8_t CPU::read_instr()
{
	return mem->read_byte(PC++);
}

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

inline uint8_t CPU::get_HL()
{
	return mem->read_byte((regs[H_REG] << 8) + regs[L_REG]);
}

inline void CPU::set_HL(uint8_t a)
{
	return mem->write_byte((regs[H_REG] << 8) + regs[L_REG], a);
}