#include "PPU.h"
#include "Memory.h"

#define INC_LY mem->write_byte(Memory::ADDR_IO_LY, mem->read_byte(Memory::ADDR_IO_LY) + 1)


void PPU::switch_mode()
{
	switch (mode)
	{
	case Mode::H_BLANK:
		INC_LY;
		if (mem->read_byte(Memory::ADDR_IO_LY) < 144)
		{
			mode = Mode::OAM_SEARCH;
			max_phase = 20 - 1;
		}
		else
		{
			mode = Mode::V_BLANK;
			max_phase = 114 - 1;
		}
		break;
	case Mode::V_BLANK:
		if (mem->read_byte(Memory::ADDR_IO_LY) < 153)
		{
			INC_LY;
		}
		else
		{
			mem->write_byte(Memory::ADDR_IO_LY, 0);
			mode = Mode::OAM_SEARCH;
			max_phase = 20 - 1;
		}
		break;
	case Mode::OAM_SEARCH:
		mode = Mode::PIXEL_DRAWING;
		max_phase = 43 - 1;
		break;
	case Mode::PIXEL_DRAWING:
		mode = Mode::H_BLANK;
		max_phase = 51 - 1;
	}
}

void PPU::execute_one_cycle()
{
	if (phase == 0)
	{
		switch_mode();
	}

	if (phase == max_phase)
	{
		phase = 0;
		clock_cycle += 4;
		return;
	}

	phase++;
	clock_cycle += 4;
}

void PPU::execute_oam_search()
{
	// LYC=LY Coincidence Interrupt
	if (mem->read_byte(Memory::ADDR_IO_LY) == mem->read_byte(Memory::ADDR_IO_LYC) &&
			mem->get_IO_flag(Memory::ADDR_IO_STAT, 6))
		mem->set_IO_flag(Memory::ADDR_IO_IF, 1);

	// Mode 2 OAM Interrupt
	if (mem->get_IO_flag(Memory::ADDR_IO_STAT, 5))
		mem->set_IO_flag(Memory::ADDR_IO_IF, 1);

	Sprite* sprites = reinterpret_cast<Sprite*>(OAM);
	static std::array<std::vector<Sprite>, 168> sorting_buffer; // 0 <= x < 168

	for (auto element : sorting_buffer)
		element.clear();
	
	uint8_t LY = mem->read_byte(Memory::ADDR_IO_LY);
	uint8_t h = mem->get_IO_flag(Memory::ADDR_IO_LCDC, 2) ? 16 : 8; // Bit 2 - OBJ (Sprite) Size(0=8x8, 1=8x16)

	for (size_t i = 0; i < SPRITES_QUANTITY; i++)
	{
		if (IN_RANGE(sprites[i].y, LY + 16, LY + 16 - h - 1) &&
			IN_RANGE(sprites[i].x, 0, 168 - 1))
			sorting_buffer[sprites[i].x].push_back(sprites[i]);
	}

	current_line_sorted_sprites.clear();

	bool finished = false;
	for (auto vector : sorting_buffer)
	{
		for (Sprite sprite : vector)
		{
			current_line_sorted_sprites.push_back(sprite);

			if (current_line_sorted_sprites.size() == MAX_SPRITES_PER_LINE)
			{
				finished = true;
				break;
			}
		}

		if (finished)
			break;
	}
}

void PPU::execute_pixel_drawing()
{
	draw_background();
	draw_sprites();
	for (size_t i = 0; i < SCREEN_WIDTH; i++)
		screen_buffer[mem->read_byte(Memory::ADDR_IO_LY)][i] = current_line_pixels[i];
}

void PPU::draw_background()
{

}

void PPU::draw_sprites()
{
	for (size_t i = 0; i < SCREEN_WIDTH; i++)
		current_line_pixels[i] = 0;

	uint8_t h = mem->get_IO_flag(Memory::ADDR_IO_LCDC, 2) ? 16 : 8; // Bit 2 - OBJ (Sprite) Size(0=8x8, 1=8x16)
	uint8_t LY = mem->read_byte(Memory::ADDR_IO_LY);

	for (int8_t i = current_line_sorted_sprites.size() - 1; i >= 0; i--)
	{
		Sprite sprite = current_line_sorted_sprites[i];

		uint8_t palette = (GET_BIT(sprite.flags, 4)) ?
			mem->read_byte(Memory::ADDR_IO_OBP1) :
			mem->read_byte(Memory::ADDR_IO_OBP0); // Bit4   Palette number  **Non CGB Mode Only** (0=OBP0, 1=OBP1)

		bool flip_x = GET_BIT(sprite.flags, 5); // Bit5   X flip (0 = Normal, 1 = Horizontally mirrored)
		bool flip_y = GET_BIT(sprite.flags, 6); // Bit6   Y flip (0 = Normal, 1 = Vertically mirrored)

		uint8_t line_address;
		if (!flip_y)
			line_address = ADDR_SPRITES_DATA_START + sprite.tile_number + (LY - (sprite.y - h));
		else
			line_address = ADDR_SPRITES_DATA_START + sprite.tile_number + (h - 1) - (LY - (sprite.y - h));

		std::array<uint8_t, 8> pixel_line = bytes_to_pixel_line(line_address);

		uint8_t start_index = (sprite.x - 8 < 0) ? (8 - sprite.x) : 0; // sprites are always 8 pixels wide
		uint8_t end_index = (sprite.x >= 160) ? (168 - sprite.x) : 8; // exclusive

		for (uint8_t pixel_idx = start_index; pixel_idx < end_index; pixel_idx++)
		{
			uint8_t colour;
			if (!flip_x)
				colour = get_colour(palette, pixel_line[pixel_idx]);
			else
				colour = get_colour(palette, pixel_line[7 - pixel_idx]);

			if (colour == 0)
				continue;

			current_line_pixels[sprite.x - 8 + pixel_idx] = colour;
		}
	}
}

std::array<uint8_t, 8> PPU::bytes_to_pixel_line(uint16_t address)
{
	uint8_t first = VRAM[address];
	uint8_t second = VRAM[address + 1];
	std::array<uint8_t, 8> res;
	for (uint8_t i = 0; i < 8; i++)
		res[7 - i] = (GET_BIT(first, i) >> i) | ((GET_BIT(second, i) >> i) << 1);
	return res;
}

std::uint8_t PPU::get_colour(uint8_t palette, uint8_t index)
{
	return (palette & (0b00000011 << (index * 2))) >> (index * 2);
}

uint8_t PPU::read_byte_VRAM(uint16_t addr)
{
	if (mode == Mode::PIXEL_DRAWING)
		return 0xff;
	else
		return VRAM[addr - Memory::ADDR_VRAM_START];
}

void PPU::write_byte_VRAM(uint16_t addr, uint8_t value)
{
	if (mode == Mode::PIXEL_DRAWING)
		return;
	VRAM[addr - Memory::ADDR_VRAM_START] = value;
}

uint8_t PPU::read_byte_OAM(uint16_t addr)
{
	if (mode == Mode::OAM_SEARCH || mode == Mode::PIXEL_DRAWING)
		return 0xff;
	else
		return OAM[addr - Memory::ADDR_OAM_START];
}

void PPU::write_byte_OAM(uint16_t addr, uint8_t value)
{
	if (mode == Mode::OAM_SEARCH || mode == Mode::PIXEL_DRAWING)
		return;
	OAM[addr - Memory::ADDR_OAM_START] = value;
}