#pragma once

#include "Generator.h"

class NoiseGenerator: public Generator
{
private:
	double  sound_length_accumulated_time = 0;

	double  sound_length = 1.0 / 256;

	uint8_t envelope_initial_volume = 0;
	int8_t  envelope_direction = -1; // 1 - increase, -1 - decrease
	double  envelope_step = 0;
	double  envelope_accumulated_time = 0;

	int8_t  current_volume = 0;

	uint8_t  counter_width = 15;
	uint16_t shift_register = 0xF13;
	double   shift_accumulated_time = 0;
	double   period = 1.0 / 524288;

	bool infinite_sound = true;
	bool turned_on = false;

public:
	void process_sound_IO_write(uint16_t addr, uint8_t value) override;

	int16_t generate_sample(double elapsed_time) override;
};