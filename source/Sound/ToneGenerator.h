#pragma once

#include <array>

#include "Generator.h"
#include "SoundMath.h"
#include "..\util.h"

class ToneGenerator: public Generator
{
public:
	ToneGenerator(bool sweep_enabled);
	void process_sound_IO_write(uint16_t addr, uint8_t value) override;

	int16_t generate_sample(double elapsed_time) override;
private:
	double  sound_length_accumulated_time = 0;
	double  sweep_accumulated_time = 0;
	double  envelope_accumulated_time = 0;

	const bool sweep_enabled;
	double  sweep_step = 0;
	int8_t  sweep_direction = 1; // 1 - increase, -1 - decrease
	uint8_t sweep_shift = 0;

	uint8_t duty = 0;
	double  sound_length = 1.0 / 256;

	uint8_t envelope_initial_volume = 0;
	int8_t  envelope_direction = -1; // 1 - increase, -1 - decrease
	double  envelope_step = 0;

	int8_t  current_volume = 0;

	uint16_t frequency_specifier = 1792; // x; frequency = 131072/(2048-x) Hz
	double   frequency = 131072.0 / (2048 - 1792);

	// flags
	bool infinite_sound = true;
	bool turned_on = false;
	bool frequecy_changed = true;
	int16_t counter = 0;

	// math
	Vec2 wt = Vec2(1, 0); // unit vector containing cosine and sine of phase
	Mat2 wdt_rotation = Mat2(0);

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
	if (frequecy_changed)
	{
		frequecy_changed = false;
		wdt_rotation = Mat2(2 * PI * frequency * elapsed_time);
	}

	static constexpr std::array<Vec2, 20> fourier_coefs =
	{
	  Vec2(0, 1.27323954f), Vec2(0, 0),
	  Vec2(0, 0.42441318f), Vec2(0, 0),
	  Vec2(0, 0.25464790f), Vec2(0, 0),
	  Vec2(0, 0.18189136f), Vec2(0, 0),
	  Vec2(0, 0.14147106f), Vec2(0, 0),
	  Vec2(0, 0.11574904f), Vec2(0, 0),
	  Vec2(0, 0.09794150f), Vec2(0, 0),
	  Vec2(0, 0.08488263f), Vec2(0, 0),
	  Vec2(0, 0.07489644f), Vec2(0, 0),
	  Vec2(0, 0.06701260f), Vec2(0, 0),
	};
	static constexpr size_t N = 20;

	wdt_rotation.mul(wt);

	Vec2 nwt(wt);
	Mat2 wt_rotation(wt);
	float sample = 0;
	for (size_t i = 0; i < N - 1; i++)
	{
		sample += fourier_coefs[i] * nwt;
		wt_rotation.mul(nwt);
	}
	sample += fourier_coefs[N - 1] * nwt;

	counter++;
	if (counter % 10000 == 0)
		wt.normalize();

	sample *= current_volume * 1000;
	return sample;
}