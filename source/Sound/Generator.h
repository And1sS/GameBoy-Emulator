#pragma once

#include <cstdint>

class APU;

class Generator 
{
public:
	Generator(APU* apu, uint8_t number);

	virtual int16_t generate_sample(double elapsed_time) { return 0; };

	virtual void process_sound_IO_write(uint16_t addr, uint8_t value) = 0;

	virtual ~Generator() {};

	void turn_off() { turned_on = false; }

protected:
	APU*    apu;
	uint8_t number;

	double  sound_length_accumulated_time = 0;
	double  sound_length = 1.0 / 256;

	bool    infinite_sound = true;
	bool    turned_on = false;

	bool handle_sound_length(double elapsed_time);
};

class MockGenerator : public Generator
{
public:
	MockGenerator(APU* apu, uint8_t number) : Generator(apu, number) {}

	void process_sound_IO_write(uint16_t addr, uint8_t value) override {}
};