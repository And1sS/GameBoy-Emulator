#pragma once

#include <cstdint>

class Generator 
{
public:
	virtual int16_t generate_sample(double elapsed_time) { return 0; };

	virtual void process_sound_IO_write(uint16_t addr, uint8_t value) = 0;

	virtual ~Generator() {};
};

class MockedGenerator : public Generator
{
public:
	void process_sound_IO_write(uint16_t addr, uint8_t value) override {}
};