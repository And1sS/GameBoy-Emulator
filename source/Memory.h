#pragma once

#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <stdexcept>
#include <istream>
#include <vector>
#include <array>

#include "util.h"

class PPU;
class APU;
class Timer;
class Cartridge;

class Memory
{
public:
	static constexpr uint16_t ADDR_IO_DIV    = 0xFF04;
	static constexpr uint16_t ADDR_IO_TIMA   = 0xFF05;
	static constexpr uint16_t ADDR_IO_TMA    = 0xFF06;
	static constexpr uint16_t ADDR_IO_TAC    = 0xFF07;

	static constexpr uint16_t ADDR_IO_IF     = 0xFF0F;

	static constexpr uint16_t ADDR_IO_NR10   = 0xFF10;
	static constexpr uint16_t ADDR_IO_NR11   = 0xFF11;
	static constexpr uint16_t ADDR_IO_NR12   = 0xFF12;
	static constexpr uint16_t ADDR_IO_NR13   = 0xFF13;
	static constexpr uint16_t ADDR_IO_NR14   = 0xFF14;
	
	static constexpr uint16_t ADDR_IO_NR21   = 0xFF16;
	static constexpr uint16_t ADDR_IO_NR22   = 0xFF17;
	static constexpr uint16_t ADDR_IO_NR23   = 0xFF18;
	static constexpr uint16_t ADDR_IO_NR24   = 0xFF19;

	static constexpr uint16_t ADDR_IO_NR41   = 0xFF20;
	static constexpr uint16_t ADDR_IO_NR42   = 0xFF21;
	static constexpr uint16_t ADDR_IO_NR43   = 0xFF22;
	static constexpr uint16_t ADDR_IO_NR44   = 0xFF23;
	
	static constexpr uint16_t ADDR_IO_NR50   = 0xFF24;
	static constexpr uint16_t ADDR_IO_NR51   = 0xFF25;
	static constexpr uint16_t ADDR_IO_NR52   = 0xFF26;

	static constexpr uint16_t ADDR_IO_LCDC   = 0xFF40;
	static constexpr uint16_t ADDR_IO_STAT   = 0xFF41;
	static constexpr uint16_t ADDR_IO_SCY    = 0xFF42;
	static constexpr uint16_t ADDR_IO_SCX    = 0xFF43;
	static constexpr uint16_t ADDR_IO_LY     = 0xFF44;
	static constexpr uint16_t ADDR_IO_LYC    = 0xFF45;
	static constexpr uint16_t ADDR_IO_BGP    = 0xFF47;
	static constexpr uint16_t ADDR_IO_OBP0   = 0xFF48;
	static constexpr uint16_t ADDR_IO_OBP1   = 0xFF49;
	static constexpr uint16_t ADDR_IO_WY     = 0xFF4A;
	static constexpr uint16_t ADDR_IO_WX     = 0xFF4B;

	static constexpr uint16_t ADDR_IE = 0xFFFF;

	// end addresses are inclusive
	static constexpr uint16_t ADDR_BANK_0_START       = 0x0000;
	static constexpr uint16_t ADDR_BANK_0_END         = 0x3FFF;
	static constexpr uint16_t ADDR_SWITCH_BANK_START  = 0x4000;
	static constexpr uint16_t ADDR_SWITCH_BANK_END    = 0x7FFF;
	static constexpr uint16_t ADDR_VRAM_START         = 0x8000;
	static constexpr uint16_t ADDR_VRAM_END           = 0x9FFF;
	static constexpr uint16_t ADDR_EXTERNAL_RAM_START = 0xA000;
	static constexpr uint16_t ADDR_EXTERNAL_RAM_END   = 0xBFFF;
	static constexpr uint16_t ADDR_OAM_START          = 0xFE00;
	static constexpr uint16_t ADDR_OAM_END            = 0xFE9F;
	static constexpr uint16_t ADDR_IO_START           = 0xFF00;
	static constexpr uint16_t ADDR_IO_END             = 0xFF7F;

	static constexpr uint16_t SIZE_OAM = 0xA0;

	static std::array<uint8_t, 256> boot_rom;
	
private:
	std::array<uint8_t, 0x10000> mem;
	Cartridge* cartridge;
	
	Timer*  timer;
	PPU*    ppu;
	APU*    apu;

	bool    boot_mode = true; // true before boot ROM is turned off
	bool    dma_transfer_mode = false;
	uint8_t machine_cycles;		// For DMA transfer only


public:
	Memory();
	Memory(std::istream& file);
	void    set_timer(Timer* timer);
	void    set_PPU(PPU* ppu);
	void    set_APU(APU* apu);

	void    execute_one_cycle();

	bool    get_dma_transfer() const;

	uint8_t read_byte(uint16_t addr) const;
	uint8_t unchecked_read_byte(uint16_t addr) const;
	void    write_byte(uint16_t addr, uint8_t value);
	void    write_two_bytes(uint16_t addr, uint16_t value);

	void    write_bytes(uint16_t addr, std::initializer_list<uint8_t> l);

	uint8_t read_IO_byte(uint16_t addr);
	void    write_IO_byte(uint16_t addr, uint8_t value);
	void    set_IO_flag(uint16_t addr, uint8_t index);
	void    reset_IO_flag(uint16_t addr, uint8_t index);
	bool    get_IO_flag(uint16_t addr, uint8_t index);

private:
	void    init();
	void    init_dma_transfer(uint8_t);
};


inline void Memory::execute_one_cycle()
{
	if (dma_transfer_mode)
	{
		machine_cycles++;
		if (machine_cycles == 160)
			dma_transfer_mode = false;
	}
}

inline bool Memory::get_dma_transfer() const
{
	return dma_transfer_mode;
}

inline uint8_t Memory::unchecked_read_byte(uint16_t addr) const
{
	return mem[addr];
}

inline uint8_t Memory::read_IO_byte(uint16_t addr)
{
#ifndef NDEBUG
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");
#endif
	return mem[addr];
}

inline void Memory::write_IO_byte(uint16_t addr, uint8_t value)
{
#ifndef NDEBUG
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");
#endif
	mem[addr] = value;
}

inline void Memory::set_IO_flag(uint16_t addr, uint8_t index)
{
#ifndef NDEBUG
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");
#endif
	mem[addr] |= (1 << index);
}

inline void Memory::reset_IO_flag(uint16_t addr, uint8_t index)
{
#ifndef NDEBUG
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");
#endif
	mem[addr] &= ~(1 << index);
}

inline bool Memory::get_IO_flag(uint16_t addr, uint8_t index)
{
#ifndef NDEBUG
	if (!IN_RANGE(addr, ADDR_IO_START, ADDR_IO_END))
		throw std::invalid_argument("address should be in IO range");
#endif
	return (mem[addr] >> index) & 1;
}

inline void Memory::init()
{
	boot_mode = false;
	mem[0xFF00] = 0x0F; // JOYPAD
	mem[0xFF05] = 0x00; // TIMA
	mem[0xFF06] = 0x00; // TMA
	mem[0xFF07] = 0x00; // TAC
	mem[0xFF10] = 0x80; // NR10
	mem[0xFF11] = 0xBF; // NR11
	mem[0xFF12] = 0xF3; // NR12
	mem[0xFF14] = 0xBF; // NR14
	mem[0xFF16] = 0x3F; // NR21
	mem[0xFF17] = 0x00; // NR22
	mem[0xFF19] = 0xBF; // NR24
	mem[0xFF1A] = 0x7F; // NR30
	mem[0xFF1B] = 0xFF; // NR31
	mem[0xFF1C] = 0x9F; // NR32
	mem[0xFF1E] = 0xBF; // NR33
	mem[0xFF20] = 0xFF; // NR41
	mem[0xFF21] = 0x00; // NR42
	mem[0xFF22] = 0x00; // NR43
	mem[0xFF23] = 0xBF; // NR44
	mem[0xFF24] = 0x77; // NR50
	mem[0xFF25] = 0xF3; // NR51
	mem[0xFF26] = 0xF1; // GB
	mem[0xFF40] = 0x91; // LCDC
	mem[0xFF42] = 0x00; // SCY
	mem[0xFF43] = 0x00; // SCX
	mem[0xFF45] = 0x00; // LYC
	mem[0xFF47] = 0xFC; // BGP
	mem[0xFF48] = 0xFF; // OBP0
	mem[0xFF49] = 0xFF; // OBP1
	mem[0xFF4A] = 0x00; // WY
	mem[0xFF4B] = 0x00; // WX
	mem[0xFFFF] = 0x00; // IE
}