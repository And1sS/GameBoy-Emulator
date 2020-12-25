#pragma once

#include <cmath>
#include <array>

#include "Generator.h"

#include "..\util.h"

class ToneGenerator: public Generator
{
private:
	const bool sweep_enabled;

	double  sweep_step = 0.0;
	int8_t  sweep_direction = 1; // 1 - increase, -1 - decrease
	uint8_t sweep_shift = 0;
	double  sweep_accumulated_time = 0;

	uint8_t duty = 0;
	double  sound_length = 1.0 / 256;

	uint8_t envelope_initial_volume = 0;
	int8_t  envelope_direction = -1; // 1 - increase, -1 - decrease
	double  envelope_step = 0;
	double  envelope_accumulated_time = 0;

	int8_t current_volume = 0;

	uint16_t frequency_specifier = 1792; // x; frequency = 131072/(2048-x) Hz
	double   period = 1.0 * (2048 - 1792) / 131072;

	bool infinite_sound = true;
	bool turned_on = false;

	static const std::array<uint8_t, 4> sound_data;

	void calculate_period();

public:
	ToneGenerator(bool sweep_enabled);
	void process_sound_IO_write(uint16_t addr, uint8_t value) override;

	int16_t generate_sample(double elapsed_time) override;
};