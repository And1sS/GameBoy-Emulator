#pragma once

#include <array>
#include <memory>

#include "Generator.h"
#include "SoundMath.h"
#include "..\util.h"

class ToneGenerator: public Generator
{
public:
	ToneGenerator(APU* apu, uint8_t number, bool sweep_enabled);
	void process_sound_IO_write(uint16_t addr, uint8_t value) override;

	int16_t generate_sample(double elapsed_time) override;
private:
	double  sweep_accumulated_time = 0;
	double  envelope_accumulated_time = 0;

	const bool sweep_enabled;
	double  sweep_step = 0;
	int8_t  sweep_direction = 1; // 1 - increase, -1 - decrease
	uint8_t sweep_shift = 0;

	uint8_t duty = 2;

	uint8_t envelope_initial_volume = 0;
	int8_t  envelope_direction = -1; // 1 - increase, -1 - decrease
	double  envelope_step = 0;

	int8_t  current_volume = 0;

	uint16_t frequency_specifier = 1792; // x; frequency = 131072/(2048-x) Hz
	double   frequency = 131072.0 / (2048 - 1792);

	bool frequency_changed = true;

	static constexpr size_t fourier_harmonics = 20;
	std::array<std::unique_ptr<FourierGeneratorBase>, 4> fourier_generators =
	{
		std::make_unique<FourierGenerator<fourier_harmonics, 0.125f>>(),
		std::make_unique<FourierGenerator<fourier_harmonics, 0.25f>>(),
		std::make_unique<FourierGenerator<fourier_harmonics, 0.5f>>(),
		std::make_unique<FourierGenerator<fourier_harmonics, 0.75f>>()
	};

	void calculate_frequency();
	void handle_sweep(double elapsed_time);
	void handle_envelope(double elapsed_time);
	int16_t _generate_sample(double elapsed_time);
};

inline void ToneGenerator::handle_sweep(double elapsed_time)
{
	sweep_accumulated_time += elapsed_time;
	while (sweep_accumulated_time >= sweep_step)
	{
		sweep_accumulated_time -= sweep_step;
		frequency_specifier += sweep_direction * (frequency_specifier >> sweep_shift);
		if (frequency_specifier > 2047)
		{
			frequency_specifier = 2047;
			turned_on = false;
		}
		calculate_frequency();
	}
}

inline void ToneGenerator::handle_envelope(double elapsed_time)
{
	envelope_accumulated_time += elapsed_time;
	while (envelope_accumulated_time >= envelope_step)
	{
		envelope_accumulated_time -= envelope_step;
		current_volume += envelope_direction;
		if (current_volume > 15)
			current_volume = 15;
		else if (current_volume < 0)
			current_volume = 0;
	}
}

// elapsed_time is assumed to be constant
inline int16_t ToneGenerator::_generate_sample(double elapsed_time)
{
	if (frequency_changed)
	{
		frequency_changed = false;
		fourier_generators[duty]->update_frequency(2 * PI * frequency);
	}

	float sample = fourier_generators[duty]->generate_sample(elapsed_time);
	sample *= current_volume * 1000;
	return sample;
}