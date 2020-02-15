#include "CPU.h"

void CPU::execute_instruction()
{
	uint8_t opcode = mem->read_byte(PC);

	//  BC, DE, HL, AF

	// LOAD INSTRUCTION SET

	// ADD INSTRUCTION SET
	if (opcode == 0x87)
	{
		regs[A_REG] = add_with_carry(regs[A_REG], regs[A_REG], false);
		clock_cycle += 4;
	}

	if (opcode >= 0x80 && opcode <= 0x85) // ADD A, r
		regs[6] += regs[opcode & 0x0F];

	
}