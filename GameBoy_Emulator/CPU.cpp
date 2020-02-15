#include "CPU.h"

void CPU::execute_instruction()
{
	uint8_t opcode = read_instr();

	// ADD INSTRUCTIONS
	
	if (opcode == 0x87)									// ADD A, A
	{
		regs[A_REG] = add_instr(regs[A_REG], regs[A_REG], false);
		clock_cycle += 4;
	}
	else if (opcode >= 0x80 && opcode <= 0x85)			// ADD A, r
	{
		regs[A_REG] += add_instr(regs[A_REG], regs[opcode & 0x0F], false);
		clock_cycle += 4;
	}
	else if (opcode == 0x86)							// ADD A, (HL)
	{
		regs[A_REG] = add_instr(regs[A_REG], get_HL(), false);
		clock_cycle += 8;
	}
	else if (opcode == 0xC6)							// ADD A, #
	{
		regs[A_REG] = add_instr(regs[A_REG], read_instr(), false);
		clock_cycle += 8;
	}
	if (opcode == 0x8F)									// ADC A, A
	{
		regs[A_REG] = add_instr(regs[A_REG], regs[A_REG], true);
		clock_cycle += 4;
	}
	else if (opcode >= 0x88 && opcode <= 0x8D)			// ADC A, r
	{
		regs[A_REG] += add_instr(regs[A_REG], regs[(opcode & 0x0F) - 8], true);
		clock_cycle += 4;
	}
	else if (opcode == 0x8E)							// ADC A, (HL)
	{
		regs[A_REG] = add_instr(regs[A_REG], get_HL(), true);
		clock_cycle += 8;
	}
	else if (opcode == 0xCE)							// ADC A, #
	{
		regs[A_REG] = add_instr(regs[A_REG], read_instr(), false);
		clock_cycle += 8;
	}	

	// SUB INSTRUCTIONS
	
	else if (opcode == 0x97)							// SUB A, A
	{
		regs[A_REG] = sub_instr(regs[A_REG], regs[A_REG], false);
		clock_cycle += 4;
	}
	else if (opcode >= 0x90 && opcode <= 0x95)			// SUB A, r
	{
		regs[A_REG] += sub_instr(regs[A_REG], regs[opcode & 0x0F], false);
		clock_cycle += 4;
	}
	else if (opcode == 0x96)							// SUB A, (HL)
	{
		regs[A_REG] = sub_instr(regs[A_REG], get_HL(), false);
		clock_cycle += 8;
	}
	else if (opcode == 0xD6)							// SUB A, #
	{
		regs[A_REG] = sub_instr(regs[A_REG], read_instr(), false);
		clock_cycle += 8;
	}
	if (opcode == 0x9F)									// SBC A, A
	{
		regs[A_REG] = sub_instr(regs[A_REG], regs[A_REG], true);
		clock_cycle += 4;
	}
	else if (opcode >= 0x98 && opcode <= 0x9D)			// SBC A, r
	{
		regs[A_REG] += sub_instr(regs[A_REG], regs[(opcode & 0x0F) - 8], true);
		clock_cycle += 4;
	}
	else if (opcode == 0x9E)							// SBC A, (HL)
	{
		regs[A_REG] = sub_instr(regs[A_REG], get_HL(), true);
		clock_cycle += 8;
	}
	else if (opcode == 0xDE)							// SBC A, #
	{
		regs[A_REG] = sub_instr(regs[A_REG], read_instr(), true);
		clock_cycle += 8;
	}	

	// AND INSTRUCTIONS
	else if (opcode == 0xA7)							// AND A, A
	{
		regs[A_REG] = and_instr(regs[A_REG], regs[A_REG]);
		clock_cycle += 4;
	}
	else if (opcode >= 0xA0 && opcode <= 0xA5)			// AND A, r
	{
		regs[A_REG] = and_instr(regs[A_REG], regs[opcode & 0x0F]);
		clock_cycle += 4;
	}
	else if (opcode == 0xA6)							// AND A, (HL)
	{
		regs[A_REG] = and_instr(regs[A_REG], get_HL());
		clock_cycle += 8;
	}
	else if (opcode == 0xE6)							// AND A, #
	{
		regs[A_REG] = and_instr(regs[A_REG], read_instr());
		clock_cycle += 8;
	}

	// OR INSTRUCTIONS
	else if (opcode == 0xB7)							// OR A, A
	{
		regs[A_REG] = or_instr(regs[A_REG], regs[A_REG]);
		clock_cycle += 4;
	}
	else if (opcode >= 0xB0 && opcode <= 0xB5)			// OR A, r
	{
		regs[A_REG] = or_instr(regs[A_REG], regs[opcode & 0x0F]);
		clock_cycle += 4;
	}
	else if (opcode == 0xB6)							// OR A, (HL)
	{
		regs[A_REG] = or_instr(regs[A_REG], get_HL());
		clock_cycle += 8;
	}
	else if (opcode == 0xF6)							// OR A, #
	{
		regs[A_REG] = or_instr(regs[A_REG], read_instr());
		clock_cycle += 8;
	}

	// XOR INSTRUCTIONS
	else if (opcode == 0xAF)							// XOR A, A
	{
		regs[A_REG] = xor_instr(regs[A_REG], regs[A_REG]);
		clock_cycle += 4;
	}
	else if (opcode >= 0xA8 && opcode <= 0xAD)			// XOR A, r
	{
		regs[A_REG] = xor_instr(regs[A_REG], regs[(opcode & 0x0F) - 8]);
		clock_cycle += 4;
	}
	else if (opcode == 0xAE)							// XOR A, (HL)
	{
		regs[A_REG] = xor_instr(regs[A_REG], get_HL());
		clock_cycle += 8;
	}
	else if (opcode == 0xEE)							// XOR A, #
	{
		regs[A_REG] = xor_instr(regs[A_REG], read_instr());
		clock_cycle += 8;
	}

	// CP INSTRUCTIONS
	else if (opcode == 0xBF)							// CP A, A
	{
		cp_instr(regs[A_REG], regs[A_REG]);
		clock_cycle += 4;
	}
	else if (opcode >= 0xB8 && opcode <= 0xBD)			// CP A, r
	{
		cp_instr(regs[A_REG], regs[(opcode & 0x0F) - 8]);
		clock_cycle += 4;
	}
	else if (opcode == 0xBE)							// CP A, (HL)
	{
		cp_instr(regs[A_REG], get_HL());
		clock_cycle += 8;
	}
	else if (opcode == 0xFE)							// CP A, #
	{
		cp_instr(regs[A_REG], read_instr());
		clock_cycle += 8;
	}

	// INC INSTRUCTIONS
	else if (opcode == 0x3C)							// INC A
	{
		regs[A_REG] = inc_instr(regs[A_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x04)							// INC B
	{
		regs[B_REG] = inc_instr(regs[B_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x0C)							// INC C
	{
		regs[C_REG] = inc_instr(regs[C_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x14)							// INC D
	{
		regs[D_REG] = inc_instr(regs[D_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x1C)							// INC E
	{
		regs[E_REG] = inc_instr(regs[E_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x24)							// INC H
	{
		regs[H_REG] = inc_instr(regs[H_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x2C)							// INC L
	{
		regs[L_REG] = inc_instr(regs[L_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x34)							// INC (HL)
	{
		set_HL(inc_instr(get_HL()));
		clock_cycle += 12;
	}

	// DEC INSTRUCTIONS
	else if (opcode == 0x3D)							// DEC A
	{
		regs[A_REG] = dec_instr(regs[A_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x05)							// DEC B
	{
		regs[B_REG] = dec_instr(regs[B_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x0D)							// DEC C
	{
		regs[C_REG] = dec_instr(regs[C_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x15)							// DEC D
	{
		regs[D_REG] = dec_instr(regs[D_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x1D)							// DEC E
	{
		regs[E_REG] = dec_instr(regs[E_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x25)							// DEC H
	{
		regs[H_REG] = dec_instr(regs[H_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x2D)							// DEC L
	{
		regs[L_REG] = dec_instr(regs[L_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x35)							// DEC (HL)
	{
		set_HL(dec_instr(get_HL()));
		clock_cycle += 12;
	}
}

uint8_t CPU::add_instr(uint8_t a, uint8_t b, bool use_carry)
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

uint8_t CPU::sub_instr(uint8_t a, uint8_t b, bool use_carry)
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

uint8_t CPU::and_instr(uint8_t a, uint8_t b)
{
	uint8_t res = a & b;
	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	reset_n_bit();
	set_h_bit();
	reset_c_bit();

	return res;
}

uint8_t CPU::or_instr(uint8_t a, uint8_t b)
{
	uint8_t res = a | b;
	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	reset_n_bit();
	reset_h_bit();
	reset_c_bit();

	return res;
}

uint8_t CPU::xor_instr(uint8_t a, uint8_t b)
{
	uint8_t res = a ^ b;
	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	reset_n_bit();
	reset_h_bit();
	reset_c_bit();

	return res;
}

void CPU::cp_instr(uint8_t a, uint8_t b)
{
	if (a == b)
		set_z_bit();
	else
		reset_z_bit();

	if (a < b)
		set_c_bit();
	else
		reset_c_bit();

	set_n_bit();

	if ((a & 0x0F) < (b & 0x0F))
		set_h_bit();
	else
		reset_h_bit();

}

uint8_t CPU::inc_instr(uint8_t a)
{
	uint8_t res = a + 1;

	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	reset_n_bit();
	
	if ((a & 0x0F) + 1 > 0x0F)
		set_h_bit();
	else
		reset_h_bit();

	return res;
}

uint8_t CPU::dec_instr(uint8_t a)
{
	uint8_t res = a - 1;

	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	set_n_bit();

	if ((a & 0x0F) == 0)
		set_h_bit();
	else
		reset_h_bit();

	return res;
}