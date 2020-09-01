#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "util.h"

static constexpr size_t KB = 1024;

class Cartridge
{
protected:
	std::vector<uint8_t> cartridge;
	std::vector<uint8_t> external_RAM;
	
	uint8_t* current_bank;

	Cartridge(std::vector<uint8_t> data) : cartridge(data), external_RAM(8 * KB, 0)
	{ current_bank = cartridge.data() + 16 * KB; }
public:
	virtual uint8_t read_byte(uint16_t addr) const;
	virtual void write_byte(uint16_t addr, uint8_t value);
};


uint8_t Cartridge::read_byte(uint16_t addr) const
{
	if (IN_RANGE(addr, Memory::ADDR_BANK_0_START, Memory::ADDR_BANK_0_END))
		return cartridge[addr];
	else if (IN_RANGE(addr, Memory::ADDR_SWITCH_BANK_START, Memory::ADDR_SWITCH_BANK_END))
		return current_bank[addr - Memory::ADDR_SWITCH_BANK_START];
	else
		return external_RAM[addr - Memory::ADDR_EXTERNAL_RAM_START];
}


void Cartridge::write_byte(uint16_t addr, uint8_t value)
{
	if (IN_RANGE(addr, Memory::ADDR_EXTERNAL_RAM_START, Memory::ADDR_EXTERNAL_RAM_END))
		external_RAM[addr - Memory::ADDR_EXTERNAL_RAM_START] = value;
}


#include "Cartridges/ROMOnly.h"
#include "Cartridges/MBC1.h"

static Cartridge* create_cartridge(std::vector<uint8_t> data)
{
	//uint8_t ram_size_flag = cartridge[0x149];
	//if (ram_size_flag != 0)
	//	throw std::runtime_error("external ram is not supported");

	//memcpy(mem.data(), cartridge.data(), 32 * KB);

	uint8_t rom_type = data[0x147]; // 0147 - Cartridge Type
	switch (rom_type) 
	{
	case 0:  // 00h  ROM ONLY
		return new ROMOnly(data);
		break;
	case 1:   // 01h  MBC1
		return new MBC1(data);
		break;
	default:
		throw std::runtime_error("unsupported rom type");
	}
}
