#include "CPU.h"
#include "Memory.h"

#define IMPL_INSTR(OPCODE, IMPLEMENTATION, CYCLES) \
	void CPU::instr_##OPCODE()                     \
	{                                              \
		IMPLEMENTATION;                            \
		clock_cycle += CYCLES;                     \
	}

// ADD INSTRUCTIONS
IMPL_INSTR(87, regs[A_REG] = add_instr(regs[A_REG], regs[A_REG], false)   ,  4)     // ADD A, A
IMPL_INSTR(80, regs[A_REG] = add_instr(regs[A_REG], regs[B_REG], false)   ,  4)     // ADD A, B
IMPL_INSTR(81, regs[A_REG] = add_instr(regs[A_REG], regs[C_REG], false)   ,  4)     // ADD A, C
IMPL_INSTR(82, regs[A_REG] = add_instr(regs[A_REG], regs[D_REG], false)   ,  4)     // ADD A, D
IMPL_INSTR(83, regs[A_REG] = add_instr(regs[A_REG], regs[E_REG], false)   ,  4)     // ADD A, E
IMPL_INSTR(84, regs[A_REG] = add_instr(regs[A_REG], regs[H_REG], false)   ,  4)     // ADD A, H
IMPL_INSTR(85, regs[A_REG] = add_instr(regs[A_REG], regs[L_REG], false)   ,  4)     // ADD A, L
IMPL_INSTR(86, regs[A_REG] = add_instr(regs[A_REG], read_from_HL(), false),  8)     // ADD A, (HL)
IMPL_INSTR(C6, regs[A_REG] = add_instr(regs[A_REG], read_ud8(), false)    ,  8)     // ADD A, ud8
																			        
IMPL_INSTR(8F, regs[A_REG] = add_instr(regs[A_REG], regs[A_REG], true)    ,  4)     // ADC A, A
IMPL_INSTR(88, regs[A_REG] = add_instr(regs[A_REG], regs[B_REG], true)    ,  4)     // ADC A, B
IMPL_INSTR(89, regs[A_REG] = add_instr(regs[A_REG], regs[C_REG], true)    ,  4)     // ADC A, C
IMPL_INSTR(8A, regs[A_REG] = add_instr(regs[A_REG], regs[D_REG], true)    ,  4)     // ADC A, D
IMPL_INSTR(8B, regs[A_REG] = add_instr(regs[A_REG], regs[E_REG], true)    ,  4)     // ADC A, E
IMPL_INSTR(8C, regs[A_REG] = add_instr(regs[A_REG], regs[H_REG], true)    ,  4)     // ADC A, H
IMPL_INSTR(8D, regs[A_REG] = add_instr(regs[A_REG], regs[L_REG], true)    ,  4)     // ADC A, L
IMPL_INSTR(8E, regs[A_REG] = add_instr(regs[A_REG], read_from_HL(), true) ,  8)     // ADC A, (HL)
IMPL_INSTR(CE, regs[A_REG] = add_instr(regs[A_REG], read_ud8(), true)     ,  8)     // ADC A, ud8
																			        
																			        
// SUB INSTRUCTIONS															        
IMPL_INSTR(97, regs[A_REG] = sub_instr(regs[A_REG], regs[A_REG], false)   ,  4)     // SUB A, A
IMPL_INSTR(90, regs[A_REG] = sub_instr(regs[A_REG], regs[B_REG], false)   ,  4)     // SUB A, B
IMPL_INSTR(91, regs[A_REG] = sub_instr(regs[A_REG], regs[C_REG], false)   ,  4)     // SUB A, C
IMPL_INSTR(92, regs[A_REG] = sub_instr(regs[A_REG], regs[D_REG], false)   ,  4)     // SUB A, D
IMPL_INSTR(93, regs[A_REG] = sub_instr(regs[A_REG], regs[E_REG], false)   ,  4)     // SUB A, E
IMPL_INSTR(94, regs[A_REG] = sub_instr(regs[A_REG], regs[H_REG], false)   ,  4)     // SUB A, H
IMPL_INSTR(95, regs[A_REG] = sub_instr(regs[A_REG], regs[L_REG], false)   ,  4)     // SUB A, L
IMPL_INSTR(96, regs[A_REG] = sub_instr(regs[A_REG], read_from_HL(), false),  8)     // SUB A, (HL)
IMPL_INSTR(D6, regs[A_REG] = sub_instr(regs[A_REG], read_ud8(), false)    ,  8)     // SUB A, ud8
																			        
IMPL_INSTR(9F, regs[A_REG] = sub_instr(regs[A_REG], regs[A_REG], true)    ,  4)     // SBC A, A
IMPL_INSTR(98, regs[A_REG] = sub_instr(regs[A_REG], regs[B_REG], true)    ,  4)     // SBC A, B
IMPL_INSTR(99, regs[A_REG] = sub_instr(regs[A_REG], regs[C_REG], true)    ,  4)     // SBC A, C
IMPL_INSTR(9A, regs[A_REG] = sub_instr(regs[A_REG], regs[D_REG], true)    ,  4)     // SBC A, D
IMPL_INSTR(9B, regs[A_REG] = sub_instr(regs[A_REG], regs[E_REG], true)    ,  4)     // SBC A, E
IMPL_INSTR(9C, regs[A_REG] = sub_instr(regs[A_REG], regs[H_REG], true)    ,  4)     // SBC A, H
IMPL_INSTR(9D, regs[A_REG] = sub_instr(regs[A_REG], regs[L_REG], true)    ,  4)     // SBC A, L
IMPL_INSTR(9E, regs[A_REG] = sub_instr(regs[A_REG], read_from_HL(), true) ,  8)     // SBC A, (HL)
IMPL_INSTR(DE, regs[A_REG] = sub_instr(regs[A_REG], read_ud8(), true)     ,  8)     // SBC A, ud8


// AND INSTRUCTIONS
IMPL_INSTR(A7, regs[A_REG] = and_instr(regs[A_REG], regs[A_REG])   ,  4)		// AND A, A
IMPL_INSTR(A0, regs[A_REG] = and_instr(regs[A_REG], regs[B_REG])   ,  4)		// AND A, B
IMPL_INSTR(A1, regs[A_REG] = and_instr(regs[A_REG], regs[C_REG])   ,  4)		// AND A, C
IMPL_INSTR(A2, regs[A_REG] = and_instr(regs[A_REG], regs[D_REG])   ,  4)		// AND A, D
IMPL_INSTR(A3, regs[A_REG] = and_instr(regs[A_REG], regs[E_REG])   ,  4)		// AND A, E
IMPL_INSTR(A4, regs[A_REG] = and_instr(regs[A_REG], regs[H_REG])   ,  4)		// AND A, H
IMPL_INSTR(A5, regs[A_REG] = and_instr(regs[A_REG], regs[L_REG])   ,  4)		// AND A, L
IMPL_INSTR(A6, regs[A_REG] = and_instr(regs[A_REG], read_from_HL()),  8)		// AND A, (HL)
IMPL_INSTR(E6, regs[A_REG] = and_instr(regs[A_REG], read_ud8())    ,  8)		// AND A, ud8
																
																
// OR INSTRUCTIONS												
IMPL_INSTR(B7, regs[A_REG] = or_instr(regs[A_REG], regs[A_REG])    ,  4)		// OR A, A
IMPL_INSTR(B0, regs[A_REG] = or_instr(regs[A_REG], regs[B_REG])    ,  4)		// OR A, B
IMPL_INSTR(B1, regs[A_REG] = or_instr(regs[A_REG], regs[C_REG])    ,  4)		// OR A, C
IMPL_INSTR(B2, regs[A_REG] = or_instr(regs[A_REG], regs[D_REG])    ,  4)		// OR A, D
IMPL_INSTR(B3, regs[A_REG] = or_instr(regs[A_REG], regs[E_REG])    ,  4)		// OR A, E
IMPL_INSTR(B4, regs[A_REG] = or_instr(regs[A_REG], regs[H_REG])    ,  4)		// OR A, H
IMPL_INSTR(B5, regs[A_REG] = or_instr(regs[A_REG], regs[L_REG])    ,  4)		// OR A, L
IMPL_INSTR(B6, regs[A_REG] = or_instr(regs[A_REG], read_from_HL()) ,  8)		// OR A, (HL)
IMPL_INSTR(F6, regs[A_REG] = or_instr(regs[A_REG], read_ud8())     ,  8)		// OR A, ud8


// XOR INSTRUCTIONS
IMPL_INSTR(AF, regs[A_REG] = xor_instr(regs[A_REG], regs[A_REG])   ,  4)		// XOR A, A
IMPL_INSTR(A8, regs[A_REG] = xor_instr(regs[A_REG], regs[B_REG])   ,  4)		// XOR A, B
IMPL_INSTR(A9, regs[A_REG] = xor_instr(regs[A_REG], regs[C_REG])   ,  4)		// XOR A, C
IMPL_INSTR(AA, regs[A_REG] = xor_instr(regs[A_REG], regs[D_REG])   ,  4)		// XOR A, D
IMPL_INSTR(AB, regs[A_REG] = xor_instr(regs[A_REG], regs[E_REG])   ,  4)		// XOR A, E
IMPL_INSTR(AC, regs[A_REG] = xor_instr(regs[A_REG], regs[H_REG])   ,  4)		// XOR A, H
IMPL_INSTR(AD, regs[A_REG] = xor_instr(regs[A_REG], regs[L_REG])   ,  4)		// XOR A, L
IMPL_INSTR(AE, regs[A_REG] = xor_instr(regs[A_REG], read_from_HL()),  8)		// XOR A, (HL)
IMPL_INSTR(EE, regs[A_REG] = xor_instr(regs[A_REG], read_ud8())    ,  8)		// XOR A, ud8
																
																
// CP INSTRUCTIONS												
IMPL_INSTR(BF, cp_instr(regs[A_REG], regs[A_REG])    ,  4)		// CP A, A
IMPL_INSTR(B8, cp_instr(regs[A_REG], regs[B_REG])    ,  4)		// CP A, B
IMPL_INSTR(B9, cp_instr(regs[A_REG], regs[C_REG])    ,  4)		// CP A, C
IMPL_INSTR(BA, cp_instr(regs[A_REG], regs[D_REG])    ,  4)		// CP A, D
IMPL_INSTR(BB, cp_instr(regs[A_REG], regs[E_REG])    ,  4)		// CP A, E
IMPL_INSTR(BC, cp_instr(regs[A_REG], regs[H_REG])    ,  4)		// CP A, H
IMPL_INSTR(BD, cp_instr(regs[A_REG], regs[L_REG])    ,  4)		// CP A, L
IMPL_INSTR(BE, cp_instr(regs[A_REG], read_from_HL()) ,  8)		// CP A, (HL)
IMPL_INSTR(FE, cp_instr(regs[A_REG], read_ud8())     ,  8)		// CP A, ud8
													
													
// INC INSTRUCTIONS									
IMPL_INSTR(3C, regs[A_REG] = inc_instr(regs[A_REG])  ,  4)		// INC A
IMPL_INSTR(04, regs[B_REG] = inc_instr(regs[B_REG])  ,  4)		// INC B
IMPL_INSTR(0C, regs[C_REG] = inc_instr(regs[C_REG])  ,  4)		// INC C
IMPL_INSTR(14, regs[D_REG] = inc_instr(regs[D_REG])  ,  4)		// INC D
IMPL_INSTR(1C, regs[E_REG] = inc_instr(regs[E_REG])  ,  4)		// INC E
IMPL_INSTR(24, regs[H_REG] = inc_instr(regs[H_REG])  ,  4)		// INC H
IMPL_INSTR(2C, regs[L_REG] = inc_instr(regs[L_REG])  ,  4)		// INC L
IMPL_INSTR(34, write_to_HL(inc_instr(read_from_HL())), 12)		// INC (HL)


// DEC INSTRUCTIONS
IMPL_INSTR(3D, regs[A_REG] = dec_instr(regs[A_REG])  ,  4)		// DEC A
IMPL_INSTR(05, regs[B_REG] = dec_instr(regs[B_REG])  ,  4)		// DEC B
IMPL_INSTR(0D, regs[C_REG] = dec_instr(regs[C_REG])  ,  4)		// DEC C
IMPL_INSTR(15, regs[D_REG] = dec_instr(regs[D_REG])  ,  4)		// DEC D
IMPL_INSTR(1D, regs[E_REG] = dec_instr(regs[E_REG])  ,  4)		// DEC E
IMPL_INSTR(25, regs[H_REG] = dec_instr(regs[H_REG])  ,  4)		// DEC H
IMPL_INSTR(2D, regs[L_REG] = dec_instr(regs[L_REG])  ,  4)		// DEC L
IMPL_INSTR(35, write_to_HL(dec_instr(read_from_HL())), 12)		// DEC (HL)


IMPL_INSTR(27, daa_instr(),  4)        // DAA
IMPL_INSTR(2F, cpl_instr(),  4)        // CPL
IMPL_INSTR(37, scf_instr(),  4)        // SCF
IMPL_INSTR(3F, ccf_instr(),  4)        // CCF
													
													
// LD INSTRUCTIONS									
IMPL_INSTR(FA, regs[A_REG] = mem->read_byte(read_d16()), 16)        // LD A, (a16)

IMPL_INSTR(3E, regs[A_REG] = read_ud8()    ,  8)        // LD A, ud8
IMPL_INSTR(06, regs[B_REG] = read_ud8()    ,  8)        // LD B, ud8
IMPL_INSTR(0E, regs[C_REG] = read_ud8()    ,  8)        // LD C, ud8
IMPL_INSTR(16, regs[D_REG] = read_ud8()    ,  8)        // LD D, ud8
IMPL_INSTR(1E, regs[E_REG] = read_ud8()    ,  8)        // LD E, ud8
IMPL_INSTR(26, regs[H_REG] = read_ud8()    ,  8)        // LD H, ud8
IMPL_INSTR(2E, regs[L_REG] = read_ud8()    ,  8)        // LD L, ud8
IMPL_INSTR(36, write_to_HL(read_ud8())     , 12)        // LD (HL), ud8

IMPL_INSTR(7F, regs[A_REG] = regs[A_REG]   ,  4)        // LD A, A
IMPL_INSTR(78, regs[A_REG] = regs[B_REG]   ,  4)        // LD A, B
IMPL_INSTR(79, regs[A_REG] = regs[C_REG]   ,  4)        // LD A, C
IMPL_INSTR(7A, regs[A_REG] = regs[D_REG]   ,  4)        // LD A, D
IMPL_INSTR(7B, regs[A_REG] = regs[E_REG]   ,  4)        // LD A, E
IMPL_INSTR(7C, regs[A_REG] = regs[H_REG]   ,  4)        // LD A, H
IMPL_INSTR(7D, regs[A_REG] = regs[L_REG]   ,  4)        // LD A, L
IMPL_INSTR(0A, regs[A_REG] = read_from_BC(),  8)        // LD A, (BC)
IMPL_INSTR(1A, regs[A_REG] = read_from_DE(),  8)        // LD A, (DE)
IMPL_INSTR(7E, regs[A_REG] = read_from_HL(),  8)        // LD A, (HL)

IMPL_INSTR(47, regs[B_REG] = regs[A_REG]   ,  4)        // LD B, A
IMPL_INSTR(40, regs[B_REG] = regs[B_REG]   ,  4)        // LD B, B
IMPL_INSTR(41, regs[B_REG] = regs[C_REG]   ,  4)        // LD B, C
IMPL_INSTR(42, regs[B_REG] = regs[D_REG]   ,  4)        // LD B, D
IMPL_INSTR(43, regs[B_REG] = regs[E_REG]   ,  4)        // LD B, E
IMPL_INSTR(44, regs[B_REG] = regs[H_REG]   ,  4)        // LD B, H
IMPL_INSTR(45, regs[B_REG] = regs[L_REG]   ,  4)        // LD B, L
IMPL_INSTR(46, regs[B_REG] = read_from_HL(),  8)        // LD B, (HL)
										
IMPL_INSTR(4F, regs[C_REG] = regs[A_REG]   ,  4)        // LD C, A
IMPL_INSTR(48, regs[C_REG] = regs[B_REG]   ,  4)        // LD C, B
IMPL_INSTR(49, regs[C_REG] = regs[C_REG]   ,  4)        // LD C, C
IMPL_INSTR(4A, regs[C_REG] = regs[D_REG]   ,  4)        // LD C, D
IMPL_INSTR(4B, regs[C_REG] = regs[E_REG]   ,  4)        // LD C, E
IMPL_INSTR(4C, regs[C_REG] = regs[H_REG]   ,  4)        // LD C, H
IMPL_INSTR(4D, regs[C_REG] = regs[L_REG]   ,  4)        // LD C, L
IMPL_INSTR(4E, regs[C_REG] = read_from_HL(),  8)        // LD C, (HL)
										
IMPL_INSTR(57, regs[D_REG] = regs[A_REG], 4)        // LD D, A
IMPL_INSTR(50, regs[D_REG] = regs[B_REG], 4)        // LD D, B
IMPL_INSTR(51, regs[D_REG] = regs[C_REG], 4)        // LD D, C
IMPL_INSTR(52, regs[D_REG] = regs[D_REG], 4)        // LD D, D
IMPL_INSTR(53, regs[D_REG] = regs[E_REG], 4)        // LD D, E
IMPL_INSTR(54, regs[D_REG] = regs[H_REG], 4)        // LD D, H
IMPL_INSTR(55, regs[D_REG] = regs[L_REG], 4)        // LD D, L
IMPL_INSTR(56, regs[D_REG] = read_from_HL(), 8)     // LD D, (HL)

IMPL_INSTR(5F, regs[E_REG] = regs[A_REG], 4)        // LD E, A
IMPL_INSTR(58, regs[E_REG] = regs[B_REG], 4)        // LD E, B
IMPL_INSTR(59, regs[E_REG] = regs[C_REG], 4)        // LD E, C
IMPL_INSTR(5A, regs[E_REG] = regs[D_REG], 4)        // LD E, D
IMPL_INSTR(5B, regs[E_REG] = regs[E_REG], 4)        // LD E, E
IMPL_INSTR(5C, regs[E_REG] = regs[H_REG], 4)        // LD E, H
IMPL_INSTR(5D, regs[E_REG] = regs[L_REG], 4)        // LD E, L
IMPL_INSTR(5E, regs[E_REG] = read_from_HL(), 8)     // LD E, (HL)

IMPL_INSTR(67, regs[H_REG] = regs[A_REG], 4)        // LD H, A
IMPL_INSTR(60, regs[H_REG] = regs[B_REG], 4)        // LD H, B
IMPL_INSTR(61, regs[H_REG] = regs[C_REG], 4)        // LD H, C
IMPL_INSTR(62, regs[H_REG] = regs[D_REG], 4)        // LD H, D
IMPL_INSTR(63, regs[H_REG] = regs[E_REG], 4)        // LD H, E
IMPL_INSTR(64, regs[H_REG] = regs[H_REG], 4)        // LD H, H
IMPL_INSTR(65, regs[H_REG] = regs[L_REG], 4)        // LD H, L
IMPL_INSTR(66, regs[H_REG] = read_from_HL(), 8)     // LD H, (HL)

IMPL_INSTR(6F, regs[L_REG] = regs[A_REG], 4)        // LD L, A
IMPL_INSTR(68, regs[L_REG] = regs[B_REG], 4)        // LD L, B
IMPL_INSTR(69, regs[L_REG] = regs[C_REG], 4)        // LD L, C
IMPL_INSTR(6A, regs[L_REG] = regs[D_REG], 4)        // LD L, D
IMPL_INSTR(6B, regs[L_REG] = regs[E_REG], 4)        // LD L, E
IMPL_INSTR(6C, regs[L_REG] = regs[H_REG], 4)        // LD L, H
IMPL_INSTR(6D, regs[L_REG] = regs[L_REG], 4)        // LD L, L
IMPL_INSTR(6E, regs[L_REG] = read_from_HL(), 8)     // LD L, (HL)

IMPL_INSTR(77, write_to_HL(regs[A_REG]), 8)        // LD (HL), A
IMPL_INSTR(70, write_to_HL(regs[B_REG]), 8)        // LD (HL), B
IMPL_INSTR(71, write_to_HL(regs[C_REG]), 8)        // LD (HL), C
IMPL_INSTR(72, write_to_HL(regs[D_REG]), 8)        // LD (HL), D
IMPL_INSTR(73, write_to_HL(regs[E_REG]), 8)        // LD (HL), E
IMPL_INSTR(74, write_to_HL(regs[H_REG]), 8)        // LD (HL), H
IMPL_INSTR(75, write_to_HL(regs[L_REG]), 8)        // LD (HL), L

IMPL_INSTR(02, write_to_BC(regs[A_REG]), 8)                                  // LD (BC), A
IMPL_INSTR(12, write_to_DE(regs[A_REG]), 8)                                  // LD (DE), A
IMPL_INSTR(EA, mem->write_byte(read_d16(), regs[A_REG]), 16)                 // LD (a16), A

IMPL_INSTR(F2, regs[A_REG] = mem->read_byte(0xFF00 + regs[C_REG]), 8)        // LD A, (FF00 + C)
IMPL_INSTR(F0, regs[A_REG] = mem->read_byte(0xFF00 + read_ud8()), 12)        // LD A, (FF00 + a8)
IMPL_INSTR(E2, mem->write_byte(0xFF00 + regs[C_REG], regs[A_REG]), 8)        // LD (FF00 + C), A
IMPL_INSTR(E0, mem->write_byte(0xFF00 + read_ud8(), regs[A_REG]), 12)        // LD (FF00 + a8), A 

IMPL_INSTR(2A, regs[A_REG] = read_from_HL(); inc_HL(), 8)        // LD A, (HL+)
IMPL_INSTR(3A, regs[A_REG] = read_from_HL(); dec_HL(), 8)        // LD A, (HL-)
IMPL_INSTR(22, write_to_HL(regs[A_REG]); inc_HL(), 8)            // LD (HL+), A
IMPL_INSTR(32, write_to_HL(regs[A_REG]); dec_HL(), 8)            // LD (HL-), A


// 16-BIT LD INSTRUCTIONS
IMPL_INSTR(08, mem->write_two_bytes(read_d16(), SP), 20)        // LD (a16), SP

IMPL_INSTR(01, set_BC(read_d16()), 12)        // LD BC, d16
IMPL_INSTR(11, set_DE(read_d16()), 12)        // LD DE, d16
IMPL_INSTR(21, set_HL(read_d16()), 12)        // LD HL, d16
IMPL_INSTR(31, SP = read_d16(), 12)           // LD SP, d16
IMPL_INSTR(F9, SP = get_HL(), 8)              // LD SP, HL
IMPL_INSTR(F8, ld_hl_spd8_instr(), 12)        // LD HL, SP + d8

IMPL_INSTR(C5, push_instr(get_BC()), 16)      // PUSH BC
IMPL_INSTR(D5, push_instr(get_DE()), 16)      // PUSH DE
IMPL_INSTR(E5, push_instr(get_HL()), 16)      // PUSH HL
IMPL_INSTR(F5, push_instr(get_AF()), 16)      // PUSH AF

IMPL_INSTR(C1, set_BC(pop_instr()), 12)       // POP BC
IMPL_INSTR(D1, set_DE(pop_instr()), 12)       // POP DE
IMPL_INSTR(E1, set_HL(pop_instr()), 12)       // POP HL
IMPL_INSTR(F1, set_AF(pop_instr()), 12)       // POP AF

// 16-BIT ALU
IMPL_INSTR(03, set_BC(get_BC() + 1), 8)       // INC BC
IMPL_INSTR(13, set_DE(get_DE() + 1), 8)       // INC DE
IMPL_INSTR(23, inc_HL(), 8)                   // INC HL
IMPL_INSTR(33, SP++, 8)                       // INC SP

IMPL_INSTR(0B, set_BC(get_BC() - 1), 8)       // DEC BC
IMPL_INSTR(1B, set_DE(get_DE() - 1), 8)       // DEC DE
IMPL_INSTR(2B, dec_HL(), 8)                   // DEC HL
IMPL_INSTR(3B, SP--, 8)                       // DEC SP

IMPL_INSTR(09, set_HL(add16_instr(get_HL(), get_BC())), 8)        // ADD HL, BC
IMPL_INSTR(19, set_HL(add16_instr(get_HL(), get_DE())), 8)        // ADD HL, DE
IMPL_INSTR(29, set_HL(add16_instr(get_HL(), get_HL())), 8)        // ADD HL, HL
IMPL_INSTR(39, set_HL(add16_instr(get_HL(), SP)), 8)              // ADD HL, SP
IMPL_INSTR(E8, add_spd8_instr(), 16)                              // ADD SP, d8


// MISCELLANIOUS
IMPL_INSTR(00, , 4)            // NOP
IMPL_INSTR(FB, IME = true, 4)  // EI
IMPL_INSTR(F3, IME = false, 4) // DI


// JUMP / CALL
IMPL_INSTR(C3, PC = read_d16(), 16)        // JP a16
IMPL_INSTR(E9, PC = get_HL(), 4)           // JP HL
IMPL_INSTR(18, PC += read_d8(), 12)        // JP r8

IMPL_INSTR(C2, if (!get_z_bit()) { PC = read_d16(); } else { PC += 2; }, (!get_z_bit()) ? 16 : 12)        // JP NZ, a16
IMPL_INSTR(CA, if (get_z_bit())  { PC = read_d16(); } else { PC += 2; }, (get_z_bit()) ? 16 : 12)         // JP Z, a16
IMPL_INSTR(D2, if (!get_c_bit()) { PC = read_d16(); } else { PC += 2; }, (!get_c_bit()) ? 16 : 12)        // JP NC, a16
IMPL_INSTR(DA, if (get_c_bit())  { PC = read_d16(); } else { PC += 2; }, (get_c_bit()) ? 16 : 12)         // JP C, a16 

IMPL_INSTR(20, if (!get_z_bit()) { PC += read_d8(); } else { PC++; }, (!get_z_bit()) ? 12 : 8)        // JR NZ, d8
IMPL_INSTR(28, if (get_z_bit())  { PC += read_d8(); } else { PC++; }, (get_z_bit()) ? 12 : 8)         // JR Z, d8
IMPL_INSTR(30, if (!get_c_bit()) { PC += read_d8(); } else { PC++; }, (!get_c_bit()) ? 12 : 8)        // JR NC, d8
IMPL_INSTR(38, if (get_c_bit())  { PC += read_d8(); } else { PC++; }, (get_c_bit()) ? 12 : 8)         // JR C, d8

IMPL_INSTR(CD, push_instr(PC + 2); PC = read_d16(), 24)        // CALL a16
IMPL_INSTR(C4, if (!get_z_bit()) { push_instr(PC + 2); PC = read_d16(); } else { PC += 2; }, (!get_z_bit()) ? 24 : 12)        // CALL NZ, a16
IMPL_INSTR(CC, if (get_z_bit())  { push_instr(PC + 2); PC = read_d16(); } else { PC += 2; }, (get_z_bit()) ? 24 : 12)         // CALL Z, a16
IMPL_INSTR(D4, if (!get_c_bit()) { push_instr(PC + 2); PC = read_d16(); } else { PC += 2; }, (!get_c_bit()) ? 24 : 12)        // CALL NC, a16
IMPL_INSTR(DC, if (get_c_bit())  { push_instr(PC + 2); PC = read_d16(); } else { PC += 2; }, (get_c_bit()) ? 24 : 12)         // CALL C, a16

IMPL_INSTR(C7, push_instr(PC); PC = 0x00, 16)    // RST 00H
IMPL_INSTR(CF, push_instr(PC); PC = 0x08, 16)    // RST 08H
IMPL_INSTR(D7, push_instr(PC); PC = 0x10, 16)    // RST 10H
IMPL_INSTR(DF, push_instr(PC); PC = 0x18, 16)    // RST 18H
IMPL_INSTR(E7, push_instr(PC); PC = 0x20, 16)    // RST 20H
IMPL_INSTR(EF, push_instr(PC); PC = 0x28, 16)    // RST 28H
IMPL_INSTR(F7, push_instr(PC); PC = 0x30, 16)    // RST 30H
IMPL_INSTR(FF, push_instr(PC); PC = 0x38, 16)    // RST 38H

IMPL_INSTR(C9, PC = pop_instr(), 16)             // RET
IMPL_INSTR(D9, IME = true; PC = pop_instr(), 16) // RETI
IMPL_INSTR(C0, if (!get_z_bit()) PC = pop_instr(), (!get_z_bit()) ? 20 : 8)    // RET NZ
IMPL_INSTR(C8, if (get_z_bit()) PC = pop_instr(), (get_z_bit()) ? 20 : 8)      // RET Z
IMPL_INSTR(D0, if (!get_c_bit()) PC = pop_instr(), (!get_c_bit()) ? 20 : 8)    // RET NC
IMPL_INSTR(D8, if (get_c_bit()) PC = pop_instr(), (get_c_bit()) ? 20 : 8)      // RET C


IMPL_INSTR(07, regs[A_REG] = rlc_instr(regs[A_REG], true), 4)  // RLCA
IMPL_INSTR(17, regs[A_REG] = rl_instr(regs[A_REG], true),  4)  // RLA
IMPL_INSTR(0F, regs[A_REG] = rrc_instr(regs[A_REG], true), 4)  // RRCA
IMPL_INSTR(1F, regs[A_REG] = rr_instr(regs[A_REG], true),  4)  // RRA



IMPL_INSTR(CB, execute_CB_instruction(), 0) // CB INSTRUCTIONS

IMPL_INSTR(76, halt_instr(), 4) // HALT

// UNSUPPORTED INSTRUCTIONS
IMPL_INSTR(10, /*throw std::runtime_error("unsupported instruction")*/, 4)
IMPL_INSTR(D3, throw std::runtime_error("unsupported instruction"), 0)
IMPL_INSTR(DB, throw std::runtime_error("unsupported instruction"), 0)
IMPL_INSTR(DD, throw std::runtime_error("unsupported instruction"), 0)
IMPL_INSTR(E3, throw std::runtime_error("unsupported instruction"), 0)
IMPL_INSTR(E4, throw std::runtime_error("unsupported instruction"), 0)
IMPL_INSTR(EB, throw std::runtime_error("unsupported instruction"), 0)
IMPL_INSTR(EC, throw std::runtime_error("unsupported instruction"), 0)
IMPL_INSTR(ED, throw std::runtime_error("unsupported instruction"), 0)
IMPL_INSTR(F4, throw std::runtime_error("unsupported instruction"), 0)
IMPL_INSTR(FC, throw std::runtime_error("unsupported instruction"), 0)
IMPL_INSTR(FD, throw std::runtime_error("unsupported instruction"), 0)

#include <iostream>
#include <iomanip>
void CPU::print_debug_info(uint8_t opcode)
{
	static const std::string opcode_names[256] =
	{
	  "NOP",
	  "LD BC,nn",
	  "LD (BC),A",
	  "INC BC",
	  "INC B",
	  "DEC B",
	  "LD B,n",
	  "RLCA",
	  "LD (nn),SP",
	  "ADD HL,BC",
	  "LD A,(BC)",
	  "DEC BC",
	  "INC C",
	  "DEC C",
	  "LD C,n",
	  "RRCA",

	  "STOP",
	  "LD DE,nn",
	  "LD (DE),A",
	  "INC DE",
	  "INC D",
	  "DEC D",
	  "LD D,n",
	  "RLA",
	  "JR n",
	  "ADD HL,DE",
	  "LD A,(DE)",
	  "DEC DE",
	  "INC E",
	  "DEC E",
	  "LD E,n",
	  "RRA",

	  "JR NZ,n",
	  "LD HL,nn",
	  "LD (HL+),A",
	  "INC HL",
	  "INC H",
	  "DEC H",
	  "LD H,n",
	  "DAA",
	  "JR Z,n",
	  "ADD HL,HL",
	  "LD A,(HLI)",
	  "DEC HL",
	  "INC L",
	  "DEC L",
	  "LD L,n",
	  "CPL",

	  "JR NC,n",
	  "LD SP,nn",
	  "LD (HL-),A",
	  "INC SP",
	  "INC (HL)",
	  "DEC (HL)",
	  "LD (HL),n",
	  "SCF",
	  "JR C,n",
	  "ADD HL,SP",
	  "LD A,(HLD)",
	  "DEC SP",
	  "INC A",
	  "DEC A",
	  "LDA,n",
	  "CCF",

	  "LD B,B",
	  "LD B,C",
	  "LD B,D",
	  "LD B,E",
	  "LD B,H",
	  "LD B,L",
	  "LD B,(HL)",
	  "LD B,A",
	  "LD C,B",
	  "LD C,C",
	  "LD C,D",
	  "LD C,E",
	  "LD C,H",
	  "LD C,L",
	  "LD C,(HL)",
	  "LD C,A",

	  "LD D,B",
	  "LD D,C",
	  "LD D,D",
	  "LD D,E",
	  "LD D,H",
	  "LD D,L",
	  "LD D,(HL)",
	  "LD D,A",
	  "LD E,B",
	  "LD E,C",
	  "LD E,D",
	  "LD E,E",
	  "LD E,H",
	  "LD E,L",
	  "LD E,(HL)",
	  "LD E,A",

	  "LD H,B",
	  "LD H,C",
	  "LD H,D",
	  "LD H,E",
	  "LD H,H",
	  "LD H,L",
	  "LD H,(HL)",
	  "LD H,A",
	  "LD L,B",
	  "LD L,C",
	  "LD L,D",
	  "LD L,E",
	  "LD L,H",
	  "LD L,L",
	  "LD L,(HL)",
	  "LD L,A",

	  "LD (HL),B",
	  "LD (HL),C",
	  "LD (HL),D",
	  "LD (HL),E",
	  "LD (HL),H",
	  "LD (HL),L",
	  "HALT",
	  "LD (HL),A",
	  "LD A,B",
	  "LD A,C",
	  "LD A,D",
	  "LD A,E",
	  "LD A,H",
	  "LD A,L",
	  "LD A,(HL)",
	  "LD A,A",

	  "ADD A,B",
	  "ADD A,C",
	  "ADD A,D",
	  "ADD A,E",
	  "ADD A,H",
	  "ADD A,L",
	  "ADD A,(HL)",
	  "ADD A,A",
	  "ADC A,B",
	  "ADC A,C",
	  "ADC A,D",
	  "ADC A,E",
	  "ADC A,H",
	  "ADC A,L",
	  "ADC A,(HL)",
	  "ADC A,A",

	  "SUB B",
	  "SUB C",
	  "SUB D",
	  "SUB E",
	  "SUB H",
	  "SUB L",
	  "SUB (HL)",
	  "SUB A",
	  "SBC A,B",
	  "SBC A,C",
	  "SBC A,D",
	  "SBC A,E",
	  "SBC A,H",
	  "SBC A,L",
	  "SBC A,(HL)",
	  "SBC A,A",

	  "AND B",
	  "AND C",
	  "AND D",
	  "AND E",
	  "AND H",
	  "AND L",
	  "AND (HL)",
	  "AND A",
	  "XOR B",
	  "XOR C",
	  "XOR D",
	  "XOR E",
	  "XOR H",
	  "XOR L",
	  "XOR (HL)",
	  "XOR A",

	  "OR B",
	  "OR C",
	  "OR D",
	  "OR E",
	  "OR H",
	  "OR L",
	  "OR (HL)",
	  "OR A",
	  "CP B",
	  "CP C",
	  "CP D",
	  "CP E",
	  "CP H",
	  "CP L",
	  "CP (HL)",
	  "CP A",

	  "RET NZ",
	  "POP BC",
	  "JP NZ,nn",
	  "JP nn",
	  "CALL NZ,nn",
	  "PUSH BC",
	  "ADD A,n",
	  "RST ",
	  "RET Z",
	  "RET",
	  "JP Z,nn",
	  "RL C",
	  "CALL Z,nn",
	  "CALL nn",
	  "ADC A,n",
	  "RST 0x08",

	  "RET NC",
	  "POP DE",
	  "JP NC,nn",
	  "unused opcode",
	  "CALL NC,nn",
	  "PUSH DE",
	  "SUB n",
	  "RST 0x10",
	  "RET C",
	  "RETI",
	  "JP C,nn",
	  "unused opcode",
	  "CALL C,nn",
	  "unused opcode",
	  "SBC A,n",
	  "RST 0x18",

	  "LD (0xFF00+n),A",
	  "POP HL",
	  "LD (0xFF00+C),A",
	  "unused opcode",
	  "unused opcode",
	  "PUSH HL",
	  "AND n",
	  "RST 0x20",
	  "ADD SP,n",
	  "JP (HL)",
	  "LD (nn),A",
	  "unused opcode",
	  "unused opcode",
	  "unused opcode",
	  "XOR n",
	  "RST 0x28",

	  "LD A,(0xFF00+n)",
	  "POP AF",
	  "LD A,(0xFF00+C)",
	  "DI",
	  "unused opcode",
	  "PUSH AF",
	  "OR n",
	  "RST 0x30",
	  "LD HL,SP",
	  "LD SP,HL",
	  "LD A,(nn)",
	  "EI",
	  "unused opcode",
	  "unused opcode",
	  "CP n",
	  "RST 0x38"
	};

	try
	{
		std::string instr = opcode_names[opcode];
		uint8_t max_string_length = 20;
		std::string spaces_string = "";
		for (size_t i = 0; i < max_string_length - instr.size(); i++)
			spaces_string += " ";

		std::cout << instr + spaces_string
			<< " PC: " << std::hex << std::setfill('0') << std::setw(2) << PC
			<< " B: " << std::hex << std::setfill('0') << std::setw(2) << (int)regs[B_REG]
			<< " C: " << std::hex << std::setfill('0') << std::setw(2) << (int)regs[C_REG]
			<< " D: " << std::hex << std::setfill('0') << std::setw(2) << (int)regs[D_REG]
			<< " E: " << std::hex << std::setfill('0') << std::setw(2) << (int)regs[E_REG]
			<< " H: " << std::hex << std::setfill('0') << std::setw(2) << (int)regs[H_REG]
			<< " L: " << std::hex << std::setfill('0') << std::setw(2) << (int)regs[L_REG]
			<< " A: " << std::hex << std::setfill('0') << std::setw(2) << (int)regs[A_REG]
			<< " F: " << std::bitset<8>(F).to_string()
			<< " LY: " << (int)mem->read_byte(Memory::ADDR_IO_LY)
			<< std::endl;
	}
	catch (...) {}
}

void CPU::execute_one_cycle()
{
	if (phase == 0)
	{
		bool interrupts_handled = handle_interrupts();

		if (!interrupts_handled && halted)
			return;

		if (!interrupts_handled)
		{
			uint64_t begin = clock_cycle;

			uint8_t opcode = read_next_instr();
 			(this->*instr_table[opcode])();
			max_phase = (clock_cycle - begin) / 4 - 1;

			clock_cycle = begin;

			//print_debug_info(opcode);
		}
		else 
			halted = false;
	}

	if (phase == max_phase)
		phase = 0;
	else
		phase++;
	clock_cycle += 4;
}

bool CPU::handle_interrupts()
{
	static uint16_t interrupt_vectors[] =
	{
		0x0040, 0x0048, 0x0050,	0x0058, 0x0060
	};

	if (!halted && !IME)
		return false;
	
	uint8_t IE = mem->read_byte(Memory::ADDR_IE); // Interrupt enable 
	uint8_t IF = mem->read_IO_byte(Memory::ADDR_IO_IF); // Interrupt flags

	for (size_t i = 0; i < 4; i++)
	{
		if ((IE & (1 << i)) && (IF & (1 << i)))
		{
			if (halted && !IME)
			{
				halted = false;
				return false;
			}
			mem->reset_IO_flag(Memory::ADDR_IO_IF, i); // disable interrupt flag
			IME = false;
			call_interrupt_instr(interrupt_vectors[i]);
			return true;
		}
	}

	return false;
}

void CPU::execute_CB_instruction()
{
	typedef uint8_t(CPU::* type_instr_CB) (uint8_t);
	static constexpr type_instr_CB instrs[8] =
	{
		& CPU::rlc_instr, & CPU::rrc_instr, & CPU::rl_instr, & CPU::rr_instr,
		& CPU::sla_instr, & CPU::sra_instr, & CPU::swap_instr, & CPU::srl_instr
	};

	uint8_t opcode = read_next_instr();
	clock_cycle += 8;

	uint8_t reg = opcode % 8;

	uint8_t operand = regs[reg];
	if (reg == 6)
	{
		operand = read_from_HL();
		clock_cycle += 8;
	}
	else if (reg == 7)
		operand = regs[A_REG];

	uint8_t result = 0;
	if (opcode < 0x40)   // SHIFT AND SWAP ISNTRUCTIONS
	{
		uint8_t instr = opcode / 8;
		result = (this->*instrs[instr])(operand);
	}
	else if (opcode < 0x80)   // BIT INSTRUCTIONS
	{
		uint8_t bit = (opcode - 0x40) / 8;
		bit_instr(operand, bit);
		return;
	}
	else if (opcode < 0xC0)   // RES INSTRUCTIONS
	{
		uint8_t bit = (opcode - 0x80) / 8;
		result = res_instr(operand, bit);
	}
	else   // SET INSTRUCTIONS
	{
		uint8_t bit = (opcode - 0xC0) / 8;
		result = set_instr(operand, bit);
	}

	if (reg < 6)
		regs[reg] = result;
	else if (reg == 6)
		write_to_HL(result);
	else if (reg == 7)
		regs[A_REG] = result;
}

uint8_t CPU::add_instr(uint8_t a, uint8_t b, bool use_carry)
{
	uint16_t a16 = a;
	uint16_t b16 = b;

	uint16_t res = a16 + b16;
	if (use_carry)
		res += get_c_bit();

	uint8_t tmp = (a16 & 0x0F) + (b16 & 0x0F);
	if (use_carry)
		tmp += get_c_bit();

	if (res > 0xFF)
		set_c_bit();
	else
		reset_c_bit();
	res &= 0xFF;

	if (tmp > 0x0F)
		set_h_bit();
	else
		reset_h_bit();

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

	if ((a & 0x0F) < (b & 0x0F) + (use_carry ? get_c_bit() : 0))
		set_h_bit();
	else
		reset_h_bit();

	if (a < b + (use_carry ? get_c_bit() : 0)) // TODO: check if not working
		set_c_bit();
	else
		reset_c_bit();

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

	if ((a & 0x0F) < 1)
		set_h_bit();
	else
		reset_h_bit();

	return res;
}

void CPU::daa_instr()
{
	if (!get_n_bit()) 
	{  
		if (get_c_bit() || regs[A_REG] > 0x99) 
		{ 
			regs[A_REG] += 0x60;
			set_c_bit();
		}
		if (get_h_bit() || (regs[A_REG] & 0x0f) > 0x09) 
			regs[A_REG] += 0x6; 
	}
	else
	{  
		if (get_c_bit()) 
			regs[A_REG] -= 0x60;

		if (get_h_bit()) 
			regs[A_REG] -= 0x6;
	}
	
	if (regs[A_REG] == 0)
		set_z_bit();
	else
		reset_z_bit();

	reset_h_bit(); 
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

void CPU::call_interrupt_instr(uint16_t addr)
{
	push_instr(PC);
	PC = addr;
	max_phase = 4;  // ?
}

void CPU::halt_instr()
{
	if (mem->read_IO_byte(Memory::ADDR_IO_IF) &
		mem->read_byte(Memory::ADDR_IE) & 0b11111)
		return; // TODO: implement hardware bug

	halted = true;
}

void CPU::push_instr(uint16_t a)
{
	mem->write_byte(--SP, (a >> 8) & 0xFF);
	mem->write_byte(--SP, a & 0xFF);
}

uint16_t CPU::pop_instr()
{
	uint16_t tmp = mem->read_byte(SP++);
	return (mem->read_byte(SP++) << 8) + tmp;
}

void CPU::add_spd8_instr()
{
	int8_t d8 = read_d8();
	uint8_t ud8 = d8;

	if ((SP & 0x0F) + (ud8 & 0x0F) > 0x0F)
		set_h_bit();
	else
		reset_h_bit();

	if ((SP & 0xFF) + ud8 > 0xFF)
		set_c_bit();
	else
		reset_c_bit();

	reset_z_bit();
	reset_n_bit();

	SP += d8;
}

void CPU::ld_hl_spd8_instr()
{
	int8_t d8 = read_d8();
	uint8_t ud8 = d8;

	if ((SP & 0x0F) + (ud8 & 0x0F) > 0x0F)
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
}

uint16_t CPU::add16_instr(uint16_t a, uint16_t b)
{
	uint8_t z_bit = get_z_bit();
	uint16_t low_byte = add_instr(a & 0xFF, b & 0xFF, false);
	uint16_t high_byte = add_instr((a & 0xFF00) >> 8, (b & 0xFF00) >> 8, true);

	if (z_bit)
		set_z_bit();
	else
		reset_z_bit();

	return (high_byte << 8) + low_byte;
}

uint8_t CPU::set_instr(uint8_t a, uint8_t bit)
{
	return a | (1 << bit);
}

uint8_t CPU::res_instr(uint8_t a, uint8_t bit)
{
	return a & ~(1 << bit);
}

void CPU::bit_instr(uint8_t a, uint8_t bit)
{
	reset_n_bit();
	set_h_bit();

	if (a & (1 << bit))
		reset_z_bit();
	else
		set_z_bit();
}

uint8_t CPU::rl_instr(uint8_t a)
{
	return rl_instr(a, false);
}

uint8_t CPU::rl_instr(uint8_t a, bool reset_z_bit)
{
	uint8_t bit_7 = (a & (1 << 7)) >> 7;
	uint8_t res = a << 1;
	res |= get_c_bit();

	if (bit_7)
		set_c_bit();
	else
		reset_c_bit();
	
	reset_n_bit();
	reset_h_bit();

	if (reset_z_bit)
		this->reset_z_bit();
	else
	{
		if (res == 0)
			set_z_bit();
		else
			this->reset_z_bit();
	}

	return res;
}

uint8_t CPU::rr_instr(uint8_t a)
{
	return rr_instr(a, false);
}

uint8_t CPU::rr_instr(uint8_t a, bool reset_z_bit)
{
	uint8_t bit_0 = a & 1;
	uint8_t res = a >> 1;
	res |= get_c_bit() << 7;

	if (bit_0)
		set_c_bit();
	else
		reset_c_bit();

	reset_n_bit();
	reset_h_bit();

	if (reset_z_bit)
		this->reset_z_bit();
	else
	{
		if (res == 0)
			set_z_bit();
		else
			this->reset_z_bit();
	}

	return res;
}

uint8_t CPU::rlc_instr(uint8_t a)
{
	return rlc_instr(a, false);
}

uint8_t CPU::rlc_instr(uint8_t a, bool reset_z_bit)
{
	uint8_t bit_7 = (a & (1 << 7)) >> 7;
	uint8_t res = a << 1;
	res |= bit_7;

	if (bit_7)
		set_c_bit();
	else
		reset_c_bit();

	reset_n_bit();
	reset_h_bit();

	if (reset_z_bit)
		this->reset_z_bit();
	else
	{
		if (res == 0)
			set_z_bit();
		else
			this->reset_z_bit();
	}

	return res;
}

uint8_t CPU::rrc_instr(uint8_t a)
{
	return rrc_instr(a, false);
}

uint8_t CPU::rrc_instr(uint8_t a, bool reset_z_bit)
{
	uint8_t bit_0 = a & 1;
	uint8_t res = a >> 1;
	res |= bit_0 << 7;

	if (bit_0)
		set_c_bit();
	else
		reset_c_bit();

	reset_n_bit();
	reset_h_bit();

	if (reset_z_bit)
		this->reset_z_bit();
	else
	{
		if (res == 0)
			set_z_bit();
		else
			this->reset_z_bit();
	}

	return res;
}

uint8_t CPU::sla_instr(uint8_t a)
{
	uint8_t bit_7 = (a & (1 << 7)) >> 7;
	uint8_t res = a << 1;

	if (bit_7)
		set_c_bit();
	else
		reset_c_bit();

	reset_n_bit();
	reset_h_bit();

	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	return res;
}

uint8_t CPU::sra_instr(uint8_t a)
{
	uint8_t bit_0 = a & 1;
	uint8_t bit_7 = a & (1 << 7);
	uint8_t res = a >> 1;
	res |= bit_7;

	if (bit_0)
		set_c_bit();
	else
		reset_c_bit();

	reset_n_bit();
	reset_h_bit();

	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	return res;
}

uint8_t CPU::srl_instr(uint8_t a)
{
	uint8_t bit_0 = a & 1;
	uint8_t res = a >> 1;

	if (bit_0)
		set_c_bit();
	else
		reset_c_bit();

	reset_n_bit();
	reset_h_bit();

	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	return res;
}

uint8_t CPU::swap_instr(uint8_t a)
{
	uint8_t upper_nibble = (a & 0xF0) >> 4;
	uint8_t res = a << 4;
	res += upper_nibble;

	reset_h_bit();
	reset_n_bit();
	reset_c_bit();

	if (res == 0)
		set_z_bit();
	else
		reset_z_bit();

	return res;
}