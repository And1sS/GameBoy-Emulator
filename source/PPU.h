#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include <array>
#include <utility>

#include "util.h"

class Memory;

class PPU
{
public:
	enum class Mode { H_BLANK, V_BLANK, OAM_SEARCH, PIXEL_DRAWING, DISABLED };


	static constexpr size_t SPRITES_QUANTITY = 40;
	static constexpr size_t MAX_SPRITES_PER_LINE = 10;

	static constexpr size_t SCREEN_WIDTH = 160;
	static constexpr size_t SCREEN_HEIGHT = 144;

	static constexpr uint16_t ADDR_SPRITES_DATA_START = 0x0000; // real address is 0x8000
	
	template<size_t size>
	using line = std::array<uint8_t, size>;

private:
	struct Sprite
	{
		uint8_t y;
		uint8_t x;
		uint8_t tile_number;
		uint8_t flags;
	};

	Memory* mem;

	Mode      mode;

	uint8_t   phase = 0;
	uint8_t   max_phase;

	line<SCREEN_WIDTH> background_line_pixels;
	line<SCREEN_WIDTH> current_line_pixels;
	std::array<line<SCREEN_WIDTH>, SCREEN_HEIGHT> screen_buffer;

	// here sprites are sorted in reversed order due to their priority
	std::vector<Sprite> current_line_sorted_sprites;

	void    switch_mode();
	void    _switch_mode_to(Mode mode);
	void    _check_new_line();

	void    execute_oam_search();
	void    execute_pixel_drawing();


	void    draw_background();
	void    draw_window();
	void    draw_sprites();

	// address into VRAM
	std::array<uint8_t, 8> bytes_to_pixel_line(uint16_t address) const;
	uint8_t get_pixel_from_tile(uint16_t tile_address, uint8_t x, uint8_t y) const;
	uint8_t get_colour(uint8_t palette, uint8_t index) const;
	inline void parse_palette(uint8_t palette, uint8_t* dest) const;
public:
	std::array<uint8_t, 0x2000> VRAM;
	std::array<uint8_t, 0xA0>   OAM;

	PPU(Memory* mem);

	void    execute_one_cycle();

	uint8_t read_byte_VRAM(uint16_t addr) const;
	uint8_t read_byte_OAM(uint16_t addr) const;
	void    write_byte_VRAM(uint16_t addr, uint8_t value);
	void    write_byte_OAM(uint16_t addr, uint8_t value);

	Mode    get_mode() const;

	auto    get_screen_buffer() -> std::array<line<SCREEN_WIDTH>, SCREEN_HEIGHT>&;
	void    get_tile_map(std::array<line<256>, 256>& screen);

	void    print_video_memory();
};

inline std::array<uint8_t, 8> PPU::bytes_to_pixel_line(uint16_t address) const
{
	uint8_t first = VRAM[address];
	uint8_t second = VRAM[address + 1];
	std::array<uint8_t, 8> res;
	for (uint8_t i = 0; i < 8; i++)
		res[7 - i] = (GET_BIT(second, i) << 1) | GET_BIT(first, i);
	return res;
}

inline uint8_t PPU::get_pixel_from_tile(uint16_t tile_address, uint8_t x, uint8_t y) const
{
	uint16_t addr = tile_address + y * 2;
	uint8_t bit = 7 - x;
	return (GET_BIT(VRAM[addr + 1], bit) << 1) | GET_BIT(VRAM[addr], bit);
}

inline void PPU::parse_palette(uint8_t palette, uint8_t* dest) const
{
	for (int i = 0; i < 4; i++)
	{
		dest[i] = palette & 0b00000011;
		palette >>= 2;
	}
}

inline uint8_t PPU::get_colour(uint8_t palette, uint8_t index) const
{
	return (palette & (0b00000011 << (index * 2))) >> (index * 2);
}

inline PPU::Mode PPU::get_mode() const
{
	return mode;
}

inline auto PPU::get_screen_buffer() -> 
	std::array<
		PPU::line<PPU::SCREEN_WIDTH>,
		PPU::SCREEN_HEIGHT
	>&
{
	return screen_buffer;
}