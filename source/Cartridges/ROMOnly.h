#pragma once

#include "../Cartridge.h"

class ROMOnly : public Cartridge
{
public:
	ROMOnly(std::vector<uint8_t> data) : Cartridge(data) { }
};