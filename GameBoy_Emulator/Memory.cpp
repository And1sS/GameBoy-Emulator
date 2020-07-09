#include "Memory.h"
#include "PPU.h"


Memory::Memory()
{
	memset(mem, 0, 0x10000);
}

void Memory::set_PPU(PPU* ppu)
{
	this->ppu = ppu;
}

uint8_t Memory::read_byte(uint16_t addr) const
{
	if (IN_RANGE(addr, ADDR_VRAM_START, ADDR_VRAM_END))
		return ppu->read_byte_VRAM(addr);
	else if (IN_RANGE(addr, ADDR_OAM_START, ADDR_OAM_END))
		return ppu->read_byte_OAM(addr);
	return mem[addr];
}

void Memory::write_byte(uint16_t addr, uint8_t value)
{
	if (IN_RANGE(addr, ADDR_VRAM_START, ADDR_VRAM_END))
		return ppu->write_byte_VRAM(addr, value);
	else if (IN_RANGE(addr, ADDR_OAM_START, ADDR_OAM_END))
		return ppu->write_byte_OAM(addr, value);
	mem[addr] = value;
}

void Memory::write_two_bytes(uint16_t addr, uint16_t value)
{
	write_byte(addr, value & 0xFF);
	write_byte(addr + 1, (value >> 8) & 0xFF);
}

void Memory::write_bytes(uint16_t addr, std::initializer_list<uint8_t> l)
{
	std::copy(l.begin(), l.end(), mem + addr);
}