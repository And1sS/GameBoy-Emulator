#pragma once

#include <cstdint>

#include "instructions_definitions.h"
#include "Memory.h"

// Flags definitions
#define Z_BIT 7
#define N_BIT 6
#define H_BIT 5
#define C_BIT 4

// Registers definitions
#define B_REG 0
#define C_REG 1
#define D_REG 2
#define E_REG 3
#define H_REG 4
#define L_REG 5
#define A_REG 6
#define F_REG 7

// TODO: IMPLEMENT CONSTRUCTOR
class CPU;
typedef void (CPU::* type_instr) (void);

class CPU
{
private:
	uint8_t   regs[8];         // BC, DE, HL, AF Registers
	uint8_t&  F = regs[F_REG]; // Flag Register
	uint16_t  SP;              // Stack Pointer
	uint16_t  PC;              // Program Counter

	uint64_t  clock_cycle = 0;

	Memory*   mem;

	DECLARE_INSTRUCTIONS
	DECLARE_INSTRUCTIONS_TABLE

	uint8_t   read_next_instr();
	uint8_t   read_ud8();
	int8_t    read_d8();
	uint16_t  read_d16();

	// Setting / resetting   flags
	uint8_t   get_z_bit();
	void      set_z_bit();
	void      reset_z_bit();

	uint8_t   get_n_bit();
	void      set_n_bit();
	void      reset_n_bit();
	
	uint8_t   get_h_bit();
	void      set_h_bit();
	void      reset_h_bit();
	
	uint8_t   get_c_bit();
	void      set_c_bit();
	void      reset_c_bit();

	void      inc_HL();
	void      dec_HL();


	// Reading from memory / writing to memory (addr is stored in register)
	uint8_t   read_from_BC();
	void      write_to_BC(uint8_t a);

	uint8_t   read_from_DE();
	void      write_to_DE(uint8_t a);
	
	uint8_t   read_from_HL();
	void      write_to_HL(uint8_t a);
	

	// Reading from registers / writing to registers
	uint16_t  get_BC();
	void      set_BC(uint16_t a);

	uint16_t  get_DE();
	void      set_DE(uint16_t a);

	uint16_t  get_HL();
	void      set_HL(uint16_t a);

	uint16_t  get_AF();
	void      set_AF(uint16_t a);

	
	// ALU Instructions
	uint8_t   add_instr(uint8_t a, uint8_t b, bool use_carry);
	uint8_t   sub_instr(uint8_t a, uint8_t b, bool use_carry);
	uint8_t   and_instr(uint8_t a, uint8_t b); 
	uint8_t   or_instr(uint8_t a, uint8_t b);
	uint8_t   xor_instr(uint8_t a, uint8_t b);
	uint8_t   inc_instr(uint8_t a);
	uint8_t   dec_instr(uint8_t a);
	uint16_t  pop_instr();
	uint16_t  add16_instr(uint16_t a, uint16_t b);

	void      push_instr(uint16_t a);
	void      cp_instr(uint8_t a, uint8_t b);
	void      daa_instr();
	void      cpl_instr();
	void      scf_instr();
	void      ccf_instr();
	void      ld_hl_spd8_instr();
	void      add_spd8_instr();
	
public:
	CPU(Memory* mem) : mem(mem) {}

	void      execute_instruction();									 
};

inline uint8_t CPU::read_next_instr()
{
	return mem->read_byte(PC++);
}

inline uint8_t CPU::read_ud8()
{ 
	return read_next_instr();
}

inline int8_t CPU::read_d8()
{
	return read_next_instr();
}

inline uint16_t CPU::read_d16()
{
	uint16_t d16 = read_ud8();
	return d16 + (read_ud8() << 8);
}

inline uint8_t CPU::get_z_bit()
{
	return (F & (1 << Z_BIT)) >> Z_BIT;
}

inline void CPU::set_z_bit()
{
	F |= 1 << Z_BIT;
}

inline void CPU::reset_z_bit()
{
	F &= ~(1 << Z_BIT);
}

inline uint8_t CPU::get_h_bit()
{
	return (F & (1 << H_BIT)) >> H_BIT;
}

inline void CPU::set_h_bit()
{
	F |= 1 << H_BIT;
}

inline void CPU::reset_h_bit()
{
	F &= ~(1 << H_BIT);
}

inline uint8_t CPU::get_n_bit()
{
	return (F & (1 << N_BIT)) >> N_BIT;
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

inline uint16_t CPU::get_BC()
{
	return (regs[B_REG] << 8) + regs[C_REG];
}

inline void CPU::set_BC(uint16_t a)
{
	regs[B_REG] = (a >> 8) & 0xFF;
	regs[C_REG] = a & 0xFF;
}

inline uint16_t CPU::get_DE()
{
	return (regs[D_REG] << 8) + regs[E_REG];
}

inline void CPU::set_DE(uint16_t a)
{
	regs[D_REG] = (a >> 8) & 0xFF;
	regs[E_REG] = a & 0xFF;
}

inline uint16_t CPU::get_HL()
{
	return (regs[H_REG] << 8) + regs[L_REG];
}

inline void CPU::set_HL(uint16_t a)
{
	regs[H_REG] = (a >> 8) & 0xFF;
	regs[L_REG] = a & 0xFF;
}

inline uint16_t CPU::get_AF()
{
	return (regs[A_REG] << 8) + regs[F_REG];
}

inline void CPU::set_AF(uint16_t a)
{
	regs[A_REG] = (a >> 8) & 0xFF;
	regs[F_REG] = a & 0xFF;
}

inline uint8_t CPU::read_from_BC()
{
	return mem->read_byte(get_BC());
}

inline void CPU::write_to_BC(uint8_t a)
{
	return mem->write_byte(get_BC(), a);
}

inline uint8_t CPU::read_from_DE()
{
	return mem->read_byte(get_DE());
}

inline void CPU::write_to_DE(uint8_t a)
{
	return mem->write_byte(get_DE(), a);
}

inline uint8_t CPU::read_from_HL()
{
	return mem->read_byte(get_HL());
}

inline void CPU::write_to_HL(uint8_t a)
{
	return mem->write_byte(get_HL(), a);
}

inline void CPU::inc_HL()
{
	set_HL(get_HL() + 1);
}

inline void CPU::dec_HL()
{
	set_HL(get_HL() - 1);
}

