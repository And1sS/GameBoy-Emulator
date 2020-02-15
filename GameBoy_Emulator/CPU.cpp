#include "CPU.h"

void CPU::execute_instruction()
{
	uint8_t opcode = read_instr();

	// ADD INSTRUCTIONS
	{
		if (opcode == 0x87)									// ADD A, A
		{
			regs[A_REG] = add_with_carry(regs[A_REG], regs[A_REG], false);
			clock_cycle += 4;
		}
		else if (opcode >= 0x80 && opcode <= 0x85)			// ADD A, r
		{
			regs[A_REG] += add_with_carry(regs[A_REG], regs[opcode & 0x0F], false);
			clock_cycle += 4;
		}
		else if (opcode == 0x86)							// ADD A, (HL)
		{
			regs[A_REG] = add_with_carry(regs[A_REG], get_HL(), false);
			clock_cycle += 8;
		}
		else if (opcode == 0xC6)							// ADD A, #
		{
			regs[A_REG] = add_with_carry(regs[A_REG], read_instr(), false);
			clock_cycle += 8;
		}
		if (opcode == 0x8F)									// ADC A, A
		{
			regs[A_REG] = add_with_carry(regs[A_REG], regs[A_REG], true);
			clock_cycle += 4;
		}
		else if (opcode >= 0x88 && opcode <= 0x8D)			// ADC A, r
		{
			regs[A_REG] += add_with_carry(regs[A_REG], regs[(opcode & 0x0F) - 8], true);
			clock_cycle += 4;
		}
		else if (opcode == 0x8E)							// ADC A, (HL)
		{
			regs[A_REG] = add_with_carry(regs[A_REG], get_HL(), true);
			clock_cycle += 8;
		}
		else if (opcode == 0xCE)							// ADC A, #
		{
			regs[A_REG] = add_with_carry(regs[A_REG], read_instr(), false);
			clock_cycle += 8;
		}
	}

	// SUB INSTRUCTIONS
	{
		if (opcode == 0x97)									// SUB A, A
		{
			regs[A_REG] = sub_with_carry(regs[A_REG], regs[A_REG], false);
			clock_cycle += 4;
		}
		else if (opcode >= 0x90 && opcode <= 0x95)			// SUB A, r
		{
			regs[A_REG] += sub_with_carry(regs[A_REG], regs[opcode & 0x0F], false);
			clock_cycle += 4;
		}
		else if (opcode == 0x96)							// SUB A, (HL)
		{
			regs[A_REG] = sub_with_carry(regs[A_REG], get_HL(), false);
			clock_cycle += 8;
		}
		else if (opcode == 0xD6)							// SUB A, #
		{
			regs[A_REG] = sub_with_carry(regs[A_REG], read_instr(), false);
			clock_cycle += 8;
		}
		if (opcode == 0x9F)									// SBC A, A
		{
			regs[A_REG] = sub_with_carry(regs[A_REG], regs[A_REG], true);
			clock_cycle += 4;
		}
		else if (opcode >= 0x98 && opcode <= 0x9D)			// SBC A, r
		{
			regs[A_REG] += sub_with_carry(regs[A_REG], regs[(opcode & 0x0F) - 8], true);
			clock_cycle += 4;
		}
		else if (opcode == 0x9E)							// SBC A, (HL)
		{
			regs[A_REG] = sub_with_carry(regs[A_REG], get_HL(), true);
			clock_cycle += 8;
		}
		else if (opcode == 0xDE)							// SBC A, #
		{
			regs[A_REG] = sub_with_carry(regs[A_REG], read_instr(), true);
			clock_cycle += 8;
		}
	}

	
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

uint8_t CPU::sub_with_carry(uint8_t a, uint8_t b, bool use_carry)
{
	uint8_t res = a - b;
	if (use_carry)
		res -= get_c_bit();

	if (a < b + (use_carry ? get_c_bit() : 0)) // TODO: check if not working
		set_c_bit();
	else
		reset_c_bit();
	
	if ((a & 0x0F) < (b & 0x0F) + (use_carry ? get_c_bit() : 0))
		set_h_bit();
	else
		reset_h_bit();

	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	set_n_bit();

	return res;
}