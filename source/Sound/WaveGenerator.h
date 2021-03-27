#pragma once

#include <array>
#include <memory>

#include "Generator.h"
#include "..\util.h"

class WaveGenerator : public Generator
{
public:
	WaveGenerator(APU* apu, uint8_t number);
	void process_sound_IO_write(uint16_t addr, uint8_t value) override;

	int16_t generate_sample(double elapsed_time) override;
private:
	bool     power_on = false;

	double   period_accumulated_time = 0;

	float    volume_multiplier = 0; // % of max volume, from 0 to 1

	uint16_t frequency_specifier = 1920; // x; frequency = 131072/(2048-x) Hz
	double   frequency = 512;
	double   period = 1.0 / frequency;

	bool     frequency_changed = true;

	std::array<uint16_t, 32> samples_array;

	void    calculate_frequency();
	int16_t _generate_sample(double elapsed_time);
};

inline int16_t WaveGenerator::_generate_sample(double elapsed_time)
{
	period_accumulated_time += elapsed_time;
	period_accumulated_time = fmodf(period_accumulated_time, period);
	float sample = samples_array[period_accumulated_time / period * 32];
	sample *= volume_multiplier * 1000;
	return sample;
}