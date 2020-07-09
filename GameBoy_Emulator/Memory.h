#pragma once

#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <stdexcept>

#ifndef IN_RANGE
#define IN_RANGE(VAL, START, INCLUSIVE_END) (((VAL) >= (START)) && ((VAL) <= (INCLUSIVE_END)))
#endif // !IN_RANGE

#ifndef GET_BIT
#define GET_BIT(VAL, INDX) ((VAL) & (1 << INDX))
#endif // !GET_BIT

class PPU;

class Memory
{
public:
	// end addresses are inclusive
	static constexpr uint16_t ADDR_IO_IF    = 0xFF0F;
	static constexpr uint16_t ADDR_IO_LCDC  = 0xFF40;
	static constexpr uint16_t ADDR_IO_STAT  = 0xFF41;
	static constexpr uint16_t ADDR_IO_SCY   = 0xFF42;
	static constexpr uint16_t ADDR_IO_SCX   = 0xFF43;
	static constexpr uint16_t ADDR_IO_LY    = 0xFF44;
	static constexpr uint16_t ADDR_IO_LYC   = 0xFF45;
	static constexpr uint16_t ADDR_IO_BGP   = 0xFF47;
	static constexpr uint16_t ADDR_IO_OBP0  = 0xFF48;
	static constexpr uint16_t ADDR_IO_OBP1  = 0xFF49;

	static constexpr uint16_t ADDR_VRAM_START = 0x8000;
	static constexpr uint16_t ADDR_VRAM_END = 0x9FFF;
	static constexpr uint16_t ADDR_OAM_START = 0xFE00;
	static constexpr uint16_t ADDR_OAM_END = 0xFE9F;
	static constexpr uint16_t ADDR_IO_START = 0xFF00;
	static constexpr uint16_t ADDR_IO_END = 0xFF7F;
	
private:
	uint8_t mem[0xFFFF];

	PPU*    ppu;

public:
	Memory();
	void    set_PPU(PPU* ppu);

	uint8_t read_byte(uint16_t addr) const;
	void    write_byte(uint16_t addr, uint8_t value);
	void    write_two_bytes(uint16_t addr, uint16_t value);

	void    write_bytes(uint16_t addr, std::initializer_list<uint8_t> l);

	uint8_t read_IO_byte(uint16_t addr);
	void    set_IO_flag(uint16_t addr, uint8_t index);
	void    reset_IO_flag(uint16_t addr, uint8_t index);
	bool    get_IO_flag(uint16_t addr, uint8_t index);
};


inline uint8_t Memory::read_IO_byte(uint16_t addr)
{
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");
	return mem[addr];
}

inline void Memory::set_IO_flag(uint16_t addr, uint8_t index)
{
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");

	mem[addr] |= (1 << index);
}

inline void Memory::reset_IO_flag(uint16_t addr, uint8_t index)
{
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");

	mem[addr] &= ~(0 << index);
}

inline bool Memory::get_IO_flag(uint16_t addr, uint8_t index)
{
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");

	return mem[addr] & (1 << index);
}