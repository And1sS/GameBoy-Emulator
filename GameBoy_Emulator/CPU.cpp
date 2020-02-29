#include "CPU.h"

void CPU::execute_instruction()
{
	uint8_t opcode = read_next_instr();
	clock_cycle += 4;

	// ADD INSTRUCTIONS
	if (opcode == 0x87)									// ADD A, A
		regs[A_REG] = add_instr(regs[A_REG], regs[A_REG], false);
	else if (opcode >= 0x80 && opcode <= 0x85)			// ADD A, r
		regs[A_REG] = add_instr(regs[A_REG], regs[opcode - 0x80], false);
	else if (opcode == 0x86)							// ADD A, (HL)
	{
		regs[A_REG] = add_instr(regs[A_REG], read_from_HL(), false);
		clock_cycle += 4;
	}
	else if (opcode == 0xC6)							// ADD A, d8
	{
		regs[A_REG] = add_instr(regs[A_REG], read_d8(), false);
		clock_cycle += 4;
	}
	else if (opcode == 0x8F)							// ADC A, A
		regs[A_REG] = add_instr(regs[A_REG], regs[A_REG], true);
	else if (opcode >= 0x88 && opcode <= 0x8D)			// ADC A, r
		regs[A_REG] = add_instr(regs[A_REG], regs[opcode - 0x88], true);
	else if (opcode == 0x8E)							// ADC A, (HL)
	{
		regs[A_REG] = add_instr(regs[A_REG], read_from_HL(), true);
		clock_cycle += 4;
	}
	else if (opcode == 0xCE)							// ADC A, d8
	{
		regs[A_REG] = add_instr(regs[A_REG], read_d8(), false);
		clock_cycle += 4;
	}	

	// SUB INSTRUCTIONS
	else if (opcode == 0x97)							// SUB A, A
		regs[A_REG] = sub_instr(regs[A_REG], regs[A_REG], false);
	else if (opcode >= 0x90 && opcode <= 0x95)			// SUB A, r
		regs[A_REG] = sub_instr(regs[A_REG], regs[opcode - 0x90], false);
	else if (opcode == 0x96)							// SUB A, (HL)
	{
		regs[A_REG] = sub_instr(regs[A_REG], read_from_HL(), false);
		clock_cycle += 4;
	}
	else if (opcode == 0xD6)							// SUB A, d8
	{
		regs[A_REG] = sub_instr(regs[A_REG], read_d8(), false);
		clock_cycle += 4;
	}
	else if (opcode == 0x9F)							// SBC A, A
		regs[A_REG] = sub_instr(regs[A_REG], regs[A_REG], true);
	else if (opcode >= 0x98 && opcode <= 0x9D)			// SBC A, r
		regs[A_REG] = sub_instr(regs[A_REG], regs[opcode - 0x98], true);
	else if (opcode == 0x9E)							// SBC A, (HL)
	{
		regs[A_REG] = sub_instr(regs[A_REG], read_from_HL(), true);
		clock_cycle += 4;
	}
	else if (opcode == 0xDE)							// SBC A, d8
	{
		regs[A_REG] = sub_instr(regs[A_REG], read_d8(), true);
		clock_cycle += 4;
	}	

	// AND INSTRUCTIONS
	else if (opcode == 0xA7)							// AND A, A
		regs[A_REG] = and_instr(regs[A_REG], regs[A_REG]);
	else if (opcode >= 0xA0 && opcode <= 0xA5)			// AND A, r
		regs[A_REG] = and_instr(regs[A_REG], regs[opcode - 0xA0]);
	else if (opcode == 0xA6)							// AND A, (HL)
	{
		regs[A_REG] = and_instr(regs[A_REG], read_from_HL());
		clock_cycle += 4;
	}
	else if (opcode == 0xE6)							// AND A, d8
	{
		regs[A_REG] = and_instr(regs[A_REG], read_d8());
		clock_cycle += 4;
	}

	// OR INSTRUCTIONS
	else if (opcode == 0xB7)							// OR A, A
		regs[A_REG] = or_instr(regs[A_REG], regs[A_REG]);
	else if (opcode >= 0xB0 && opcode <= 0xB5)			// OR A, r
		regs[A_REG] = or_instr(regs[A_REG], regs[opcode - 0xB0]);
	else if (opcode == 0xB6)							// OR A, (HL)
	{
		regs[A_REG] = or_instr(regs[A_REG], read_from_HL());
		clock_cycle += 4;
	}
	else if (opcode == 0xF6)							// OR A, d8
	{
		regs[A_REG] = or_instr(regs[A_REG], read_d8());
		clock_cycle += 4;
	}

	// XOR INSTRUCTIONS
	else if (opcode == 0xAF)							// XOR A, A
		regs[A_REG] = xor_instr(regs[A_REG], regs[A_REG]);
	else if (opcode >= 0xA8 && opcode <= 0xAD)			// XOR A, r
		regs[A_REG] = xor_instr(regs[A_REG], regs[opcode - 0xA8]);
	else if (opcode == 0xAE)							// XOR A, (HL)
	{
		regs[A_REG] = xor_instr(regs[A_REG], read_from_HL());
		clock_cycle += 4;
	}
	else if (opcode == 0xEE)							// XOR A, d8
	{
		regs[A_REG] = xor_instr(regs[A_REG], read_d8());
		clock_cycle += 4;
	}

	// CP INSTRUCTIONS
	else if (opcode == 0xBF)							// CP A, A
		cp_instr(regs[A_REG], regs[A_REG]);
	else if (opcode >= 0xB8 && opcode <= 0xBD)			// CP A, r
		cp_instr(regs[A_REG], regs[opcode - 0xB8]);
	else if (opcode == 0xBE)							// CP A, (HL)
	{
		cp_instr(regs[A_REG], read_from_HL());
		clock_cycle += 4;
	}
	else if (opcode == 0xFE)							// CP A, d8
	{
		cp_instr(regs[A_REG], read_d8());
		clock_cycle += 4;
	}

	// INC INSTRUCTIONS
	else if (opcode == 0x3C)							// INC A
		regs[A_REG] = inc_instr(regs[A_REG]);
	else if (opcode == 0x04)							// INC B
		regs[B_REG] = inc_instr(regs[B_REG]);
	else if (opcode == 0x0C)							// INC C
		regs[C_REG] = inc_instr(regs[C_REG]);
	else if (opcode == 0x14)							// INC D
		regs[D_REG] = inc_instr(regs[D_REG]);
	else if (opcode == 0x1C)							// INC E
		regs[E_REG] = inc_instr(regs[E_REG]);
	else if (opcode == 0x24)							// INC H
		regs[H_REG] = inc_instr(regs[H_REG]);
	else if (opcode == 0x2C)							// INC L
		regs[L_REG] = inc_instr(regs[L_REG]);
	else if (opcode == 0x34)							// INC (HL)
	{
		write_to_HL(inc_instr(read_from_HL()));
		clock_cycle += 8;
	}

	// DEC INSTRUCTIONS
	else if (opcode == 0x3D)							// DEC A
		regs[A_REG] = dec_instr(regs[A_REG]);
	else if (opcode == 0x05)							// DEC B
		regs[B_REG] = dec_instr(regs[B_REG]);
	else if (opcode == 0x0D)							// DEC C
		regs[C_REG] = dec_instr(regs[C_REG]);
	else if (opcode == 0x15)							// DEC D
		regs[D_REG] = dec_instr(regs[D_REG]);
	else if (opcode == 0x1D)							// DEC E
		regs[E_REG] = dec_instr(regs[E_REG]);
	else if (opcode == 0x25)							// DEC H
		regs[H_REG] = dec_instr(regs[H_REG]);
	else if (opcode == 0x2D)							// DEC L
		regs[L_REG] = dec_instr(regs[L_REG]);
	else if (opcode == 0x35)							// DEC (HL)
	{
		write_to_HL(dec_instr(read_from_HL()));
		clock_cycle += 8;
	}
	else if (opcode == 0x27)							// DAA
		daa_instr();
	else if (opcode == 0x2F)							// CPL
		cpl_instr();
	else if (opcode == 0x37)							// SCF
		scf_instr();
	else if (opcode == 0x3F)							// CCF
		ccf_instr();

	// LD INSTRUCTIONS
	else if (opcode == 0x06)							// LD B, d8
	{
		regs[B_REG] = read_d8();
		clock_cycle += 4;
	}
	else if (opcode == 0x0E)							// LD C, d8
	{
		regs[C_REG] = read_d8();
		clock_cycle += 4;
	}
	else if (opcode == 0x16)							// LD D, d8
	{
		regs[D_REG] = read_d8();
		clock_cycle += 4;
	}
	else if (opcode == 0x1E)							// LD E, d8
	{
		regs[E_REG] = read_d8();
		clock_cycle += 4;
	}
	else if (opcode == 0x26)							// LD H, d8
	{
		regs[H_REG] = read_d8();
		clock_cycle += 4;
	}
	else if (opcode == 0x2E)							// LD L, d8
	{
		regs[L_REG] = read_d8();	
		clock_cycle += 4;
	}
	else if (opcode == 0x36)							// LD (HL), d8
	{
		write_to_HL(read_d8());
		clock_cycle += 8;
	}
	else if (opcode == 0x3E)							// LD A, d8
	{
		regs[A_REG] = read_d8();
		clock_cycle += 4;
	}
	else if (opcode == 0x7F);							// LD A, A
	else if (opcode >= 0x78 && opcode <= 0x7D)			// LD A, r
		regs[A_REG] = regs[opcode - 0x78];
	else if (opcode == 0x0A)							// LD A, (BC)
	{
		regs[A_REG] = read_from_BC();
		clock_cycle += 4;
	}
	else if (opcode == 0x1A)							// LD A, (DE)
	{
		regs[A_REG] = read_from_DE();
		clock_cycle += 4;
	}
	else if (opcode == 0x7E)							// LD A, (HL)
	{
		regs[A_REG] = read_from_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0xFA)							// LD A, (a16)
	{
		regs[A_REG] = mem->read_byte(read_d16());
		clock_cycle += 12;
	}
	else if (opcode == 0x47)							// LD B, A
		regs[B_REG] = regs[A_REG];
	else if (opcode >= 0x40 && opcode <= 0x45)			// LD B, r
		regs[B_REG] = regs[opcode - 0x40];
	else if (opcode == 0x46)							// LD B, (HL)
	{
		regs[B_REG] = read_from_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x4F)							// LD C, A
		regs[C_REG] = regs[A_REG];
	else if (opcode >= 0x48 && opcode <= 0x4D)			// LD C, r
		regs[C_REG] = regs[opcode - 0x48];
	else if (opcode == 0x4E)							// LD C, (HL)
	{
		regs[C_REG] = read_from_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x57)							// LD D, A
		regs[D_REG] = regs[A_REG];
	else if (opcode >= 0x50 && opcode <= 0x55)			// LD D, r
		regs[D_REG] = regs[opcode - 0x50];
	else if (opcode == 0x56)							// LD D, (HL)
	{
		regs[D_REG] = read_from_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x5F)							// LD E, A
		regs[E_REG] = regs[A_REG];
	else if (opcode >= 0x58 && opcode <= 0x5D)			// LD E, r
		regs[E_REG] = regs[opcode - 0x58];
	else if (opcode == 0x5E)							// LD E, (HL)
	{
		regs[E_REG] = read_from_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x67)							// LD H, A
		regs[H_REG] = regs[A_REG];
	else if (opcode >= 0x60 && opcode <= 0x65)			// LD H, r
		regs[H_REG] = regs[opcode - 0x60];
	else if (opcode == 0x66)							// LD H, (HL)
	{
		regs[H_REG] = read_from_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x6F)							// LD L, A
		regs[L_REG] = regs[A_REG];
	else if (opcode >= 0x68 && opcode <= 0x6D)			// LD L, r
		regs[L_REG] = regs[opcode - 0x68];
	else if (opcode == 0x6E)							// LD L, (HL)
	{
		regs[L_REG] = read_from_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x77)							// LD (HL), A
	{
		write_to_HL(regs[A_REG]);
		clock_cycle += 4;
	}
	else if (opcode >= 0x70 && opcode <= 0x75)			// LD (HL), r
	{
		write_to_HL(regs[opcode - 0x70]);
		clock_cycle += 4;
	}
	else if (opcode == 0x02)							// LD (BC), A
	{
		write_to_BC(regs[A_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x12)							// LD (DE), A
	{
		write_to_DE(regs[A_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0xEA)							// LD (a16), A
	{
		mem->write_byte(read_d16(), regs[A_REG]);
		clock_cycle += 12;
	}	
	else if (opcode == 0xF2)							// LD A, (FF00 + C)
	{
		regs[A_REG] = mem->read_byte(0xFF00 + regs[C_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0xE2)							// LD (FF00 + C), A
	{
		mem->write_byte(0xFF00 + regs[C_REG], regs[A_REG]);
		clock_cycle += 4;
	}
	else if (opcode == 0x3A)							// LD A, (HL-)
	{
		regs[A_REG] = read_from_HL();
		dec_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x32)							// LD (HL-), A
	{
		write_to_HL(regs[A_REG]);
		dec_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x2A)							// LD A, (HL+)
	{
		regs[A_REG] = read_from_HL();
		inc_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x22)							// LD (HL+), A
	{
		write_to_HL(regs[A_REG]);
		inc_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0xF0)							// LD A, (FF00 + a8)
	{
		regs[A_REG] = mem->read_byte(0xFF00 + read_d8());
		clock_cycle += 8;
	}
	else if (opcode == 0xE0)							// LD (FF00 + a8), A
	{
		mem->write_byte(0xFF00 + read_d8(), regs[A_REG]);
		clock_cycle += 8;
	}

	// 16-BIT LD INSTRUCTIONS
	else if (opcode == 0x01)							// LD BC, d16
	{
		set_BC(read_d16());
		clock_cycle += 8;
	}
	else if (opcode == 0x11)							// LD DE, d16
	{
		set_DE(read_d16());
		clock_cycle += 8;
	}
	else if (opcode == 0x21)							// LD HL, d16
	{
		set_HL(read_d16());
		clock_cycle += 8;
	}
	else if (opcode == 0x31)							// LD SP, d16
	{
		SP = read_d16();
		clock_cycle += 8;
	}
	else if (opcode == 0xF9)							// LD SP, HL
	{
		SP = get_HL();
		clock_cycle += 8;
	}
	else if (opcode == 0xF8)							// LD HL, SP + d8
	{
		int8_t d8 = read_d8();
		uint8_t ud8 = d8;
		
		if ((ud8 & 0x0F) + (SP & 0x0F) > 0x0F)
			set_h_bit();
		else
			reset_h_bit();

		if ((SP & 0xFF) + ud8 > 0xFF)
			set_c_bit();
		else
			reset_c_bit();
		reset_z_bit();
		reset_n_bit();

		set_HL(SP + d8);
		clock_cycle += 8;
	}
	else if (opcode == 0x08)							// LD (a16), SP
	{
		mem->write_two_bytes(read_d16(), SP);
		clock_cycle += 16;
	}
	else if (opcode == 0xC5)							// PUSH BC
	{
		push_instr(get_BC());
		clock_cycle += 12;
	}
	else if (opcode == 0xD5)							// PUSH DE
	{
		push_instr(get_DE());
		clock_cycle += 12;
	}
	else if (opcode == 0xE5)							// PUSH HL
	{
		push_instr(get_HL());
		clock_cycle += 12;
	}
	else if (opcode == 0xF5)							// PUSH AF
	{
		push_instr(get_HL());
		clock_cycle += 12;
	}
	else if (opcode == 0xC1)							// POP BC	
	{
		set_BC(pop_instr());
		clock_cycle += 8;
	}
	else if (opcode == 0xD1)							// POP DE
	{
		set_DE(pop_instr());
		clock_cycle += 8;
	}
	else if (opcode == 0xE1)							// POP HL
	{
		set_HL(pop_instr());
		clock_cycle += 8;
	}
	else if (opcode == 0xF1)							// POP AF
	{
		set_AF(pop_instr());
		clock_cycle += 8;
	}

	// 16-BIT ALU
	else if	(opcode == 0x03)							// INC BC
	{
		set_BC(get_BC() + 1);
		clock_cycle += 4;
	}
	else if (opcode == 0x13)							// INC DE
	{
		set_DE(get_DE() + 1);
		clock_cycle += 4;
	}
	else if (opcode == 0x23)							// INC HL
	{
		inc_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x33)							// INC SP
	{
		SP++;
		clock_cycle += 4;
	}
	else if (opcode == 0x0B)							// DEC BC
	{
		set_BC(get_BC() - 1);
		clock_cycle += 4;
	}
	else if (opcode == 0x1B)							// DEC DE
	{
		set_DE(get_DE() - 1);
		clock_cycle += 4;
	}
	else if (opcode == 0x2B)							// DEC HL
	{
		dec_HL();
		clock_cycle += 4;
	}
	else if (opcode == 0x3B)							// DEC SP
	{
		SP--;
		clock_cycle += 4;
	}

	else
		throw "Unimplemented instruction";
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

void CPU::daa_instr()
{
	uint8_t old_A = regs[A_REG];
	uint8_t old_carry = get_c_bit();
	if (get_n_bit() == 0)
	{
		if (((regs[A_REG] & 0x0F) > 9) || get_h_bit())
		{
			uint16_t temp = regs[A_REG] + 6;
			regs[A_REG] = temp & 0xFF;

			if (old_carry || (temp > 0xFF))
				set_c_bit();
			else
				reset_c_bit();
		}
		if ((old_A > 0x99) || get_c_bit())
		{
			regs[A_REG] += 0x60;
			set_c_bit();
		}
		else
			reset_c_bit();
	}
	else
	{
		if (((regs[A_REG] & 0x0F) > 9) || get_h_bit())
		{
			uint16_t temp = regs[A_REG] - 6;
			regs[A_REG] = temp & 0xFF;

			if (old_carry || (temp < 0))
				set_c_bit();
			else
				reset_c_bit();
		}
		if ((old_A > 0x99) || get_c_bit())
		{
			regs[A_REG] -= 0x60;
			set_c_bit();
		}
	}

	if (regs[A_REG] == 0)
		set_z_bit();
	else
		reset_z_bit();

	set_n_bit();
}

void CPU::cpl_instr()
{
	regs[A_REG] = ~regs[A_REG];

	set_n_bit();
	set_h_bit();
}

void CPU::scf_instr()
{
	set_c_bit();
	reset_n_bit();
	reset_h_bit();
}

void CPU::ccf_instr()
{
	F ^= 1 << C_BIT;

	reset_n_bit();
	reset_h_bit();
}

void CPU::push_instr(uint16_t a)
{
	mem->write_byte(SP--, a & 0xFF);
	mem->write_byte(SP--, (a >> 8) & 0xFF);
}

uint16_t CPU::pop_instr()
{
	uint16_t tmp = mem->read_byte(++SP);
	return (tmp << 8) + mem->read_byte(++SP);
}
