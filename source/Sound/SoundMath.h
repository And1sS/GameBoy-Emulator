#pragma once

#include "../gcem/gcem.hpp"

constexpr float PI = 3.14159265358;

class Vec2
{
public:
	float x, y;
	constexpr Vec2() : x(0), y(0) {}
	constexpr Vec2(float x, float y) : x(x), y(y) {}

	float operator*(const Vec2& v) const
	{
		return x * v.x + y * v.y;
	}

	void normalize()
	{
		float l = 1 / std::sqrtf(x * x + y * y);
		x *= l;
		y *= l;
	}
};

class Mat2
{
private:
	float a11, a12, a21, a22;
public:
	// rotation matrix with given angle
	Mat2(float angle)
	{
		a11 = std::cos(angle); a12 = -std::sin(angle);
		a21 = std::sin(angle); a22 = std::cos(angle);
	}

	// rotation matrix from unit vector
	Mat2(const Vec2& v)
	{
		a11 = v.x; a12 = -v.y;
		a21 = v.y; a22 = v.x;
	}

	void mul(Vec2& v) const
	{
		float oldvx = v.x;
		v.x = a11 * v.x + a12 * v.y;
		v.y = a21 * oldvx + a22 * v.y;
	}
};

namespace
{

	constexpr auto fourier_coef(size_t i, float duty)
	{
		return Vec2(1.0f / PI / (i + 1) * gcem::sin(2 * PI * (i + 1) * duty),
			1.0f / PI / (i + 1) * (1 - gcem::cos(2 * PI * (i + 1) * duty)));
	}

	template <std::size_t... i>
	constexpr auto _generate_fourier_coefs(std::index_sequence<i...>, float duty)
	{
		return std::array<Vec2, sizeof...(i)>{ fourier_coef(i, duty)... };
	}

	template <std::size_t N>
	constexpr auto generate_fourier_coefs(float duty)
	{
		return _generate_fourier_coefs(std::make_index_sequence<N>(), duty);
	}
}

class FourierGeneratorBase
{
public:
	virtual void update_frequency(float w) = 0;
	virtual void reset() = 0;
	virtual float generate_sample(float elapsed_time)
	{
		return 0;
	}
	virtual ~FourierGeneratorBase() {};
};

template <size_t N, float duty>
class FourierGenerator : public FourierGeneratorBase
{
public:
	void reset() override
	{
		wt = Vec2(1, 0);
	}

	void update_frequency(float w) override
	{
		this->w = w;
		wdt_rotation = Mat2(w * prev_elapsed_time);
	}

	float generate_sample(float elapsed_time) override
	{
		if (fabsf(elapsed_time - prev_elapsed_time) > 1e-7)
		{
			prev_elapsed_time = elapsed_time;
			wdt_rotation = Mat2(w * elapsed_time);
		}

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
		if (counter == 10000)
		{
			counter = 0;
			wt.normalize();
		}

		return sample;
	}

private:
	static constexpr auto fourier_coefs = generate_fourier_coefs<N>(duty);

	float w; // frequency
	Vec2 wt{ 1, 0 }; // unit vector containing cosine and sine of phase
	Mat2 wdt_rotation{ 0 };
	uint16_t counter = 0;

	float prev_elapsed_time = 0;
};