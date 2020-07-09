#include "PPU.h"
#include "Memory.h"

#define INC_LY mem->write_byte(Memory::ADDR_IO_LY, mem->read_byte(Memory::ADDR_IO_LY) + 1)

PPU::PPU(Memory* mem) : mem(mem)
{
	memset(VRAM, 0, 0x2000);
	memset(OAM, 0, 0xA0);

	mode = Mode::DISABLED;
}

void PPU::switch_mode()
{
	switch (mode)
	{
	case Mode::DISABLED:
		mem->write_byte(Memory::ADDR_IO_LY, 0);
		_switch_mode_to(Mode::OAM_SEARCH);
		break;
	case Mode::H_BLANK:
		INC_LY;
		if (mem->read_byte(Memory::ADDR_IO_LY) < 144)
			_switch_mode_to(Mode::OAM_SEARCH);
		else
			_switch_mode_to(Mode::V_BLANK);
		break;
	case Mode::V_BLANK:
		if (mem->read_byte(Memory::ADDR_IO_LY) < 153)
			INC_LY;
		else
		{
			mem->write_byte(Memory::ADDR_IO_LY, 0);
			_switch_mode_to(Mode::OAM_SEARCH);
		}
		break;
	case Mode::OAM_SEARCH:
		_switch_mode_to(Mode::PIXEL_DRAWING);
		break;
	case Mode::PIXEL_DRAWING:
		_switch_mode_to(Mode::H_BLANK);
	}
}

void PPU::_switch_mode_to(Mode mode)
{
	this->mode = mode;
	switch (mode)
	{
	case Mode::H_BLANK:
		max_phase = 51 - 1;
		mem->reset_IO_flag(Memory::ADDR_IO_STAT, 0);
		mem->reset_IO_flag(Memory::ADDR_IO_STAT, 1);
		break;
	case Mode::V_BLANK:
		max_phase = 114 - 1;
		mem->set_IO_flag(Memory::ADDR_IO_STAT, 0);
		mem->reset_IO_flag(Memory::ADDR_IO_STAT, 1);
		break;
	case Mode::OAM_SEARCH:
		max_phase = 20 - 1;
		mem->reset_IO_flag(Memory::ADDR_IO_STAT, 0);
		mem->set_IO_flag(Memory::ADDR_IO_STAT, 1);
		execute_oam_search();
		break;
	case Mode::PIXEL_DRAWING:
		max_phase = 43 - 1;
		mem->set_IO_flag(Memory::ADDR_IO_STAT, 0);
		mem->set_IO_flag(Memory::ADDR_IO_STAT, 1);
		execute_pixel_drawing();
	}
}

void PPU::execute_one_cycle()
{
	if (mode == Mode::DISABLED)
	{
		if (!mem->get_IO_flag(Memory::ADDR_IO_LCDC, 7)) // Bit 7 - LCD Display Enable (0 = Off, 1 = On)
			return;
		else
			phase = 0;
	}
	else if (!mem->get_IO_flag(Memory::ADDR_IO_LCDC, 7))
	{
		mode = Mode::DISABLED;
		return;
	}

	if (phase == 0)
		switch_mode();

	if (phase == max_phase)
		phase = 0;
	else
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
	static std::array<std::vector<Sprite>, 256> sorting_buffer;

	for (auto element : sorting_buffer)
		element.clear();

	uint8_t LY = mem->read_byte(Memory::ADDR_IO_LY);
	uint8_t h = mem->get_IO_flag(Memory::ADDR_IO_LCDC, 2) ? 16 : 8; // Bit 2 - OBJ (Sprite) Size(0 = 8x8, 1 = 8x16)

	size_t sprites_added = 0;
	for (size_t i = 0; i < SPRITES_QUANTITY; i++)
		if (IN_RANGE(sprites[i].y, LY + 16, LY + 16 - h - 1))
		{
			sorting_buffer[sprites[i].x].push_back(sprites[i]);
			sprites_added++;
			if (sprites_added == MAX_SPRITES_PER_LINE)
				break;
		}

	current_line_sorted_sprites.clear();

	for (auto vector : sorting_buffer)
		for (Sprite sprite : vector)
			if (IN_RANGE(sprite.x, 1, 167))
				current_line_sorted_sprites.push_back(sprite);
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
	// all coordinates are uint8_t (% 256), so that background wraps around the screen 

	uint8_t LY  = mem->read_IO_byte(Memory::ADDR_IO_LY);
	uint8_t SCX = mem->read_IO_byte(Memory::ADDR_IO_SCX);
	uint8_t SCY = mem->read_IO_byte(Memory::ADDR_IO_SCY);

	for (uint8_t pixel_x = 0; pixel_x < SCREEN_WIDTH; pixel_x++)
	{
		// integer division used on purpose
		uint16_t tile_idx = 32 * (((SCY + LY) % 256) / 8) + ((SCX + pixel_x) % 256) / 8;

		uint8_t tile;
		if (mem->get_IO_flag(Memory::ADDR_IO_LCDC, 3)) // Bit 3 - BG Tile Map Display Select  (0 = 9800-9BFF, 1 = 9C00-9FFF)
			tile = VRAM[0x9C00 - Memory::ADDR_VRAM_START + tile_idx];
		else
			tile = VRAM[0x9800 - Memory::ADDR_VRAM_START + tile_idx];

		uint16_t tile_address;
		if (mem->get_IO_flag(Memory::ADDR_IO_LCDC, 4)) // Bit 4 - BG & Window Tile Data Select (0 = 8800-97FF, 1 = 8000-8FFF)
			tile_address = 0x8000 + tile * 16 - Memory::ADDR_VRAM_START;
		else
			tile_address = 0x9000 + ((int8_t)tile) * 16 - Memory::ADDR_VRAM_START;

		uint8_t pixel_idx = get_pixel_from_tile(tile_address, (SCX + pixel_x) % 8, (SCY + LY) % 8);
		uint8_t pixel_colour = get_colour(mem->read_IO_byte(Memory::ADDR_IO_BGP), pixel_idx);
		current_line_pixels[pixel_x] = pixel_colour;
	}
}

void PPU::draw_sprites()
{
	if (!mem->get_IO_flag(Memory::ADDR_IO_LCDC, 1)) // Bit 1 - OBJ (Sprite) Display Enable(0 = Off, 1 = On)
		return;

	for (size_t i = 0; i < SCREEN_WIDTH; i++)
		current_line_pixels[i] = 0;

	uint8_t h = mem->get_IO_flag(Memory::ADDR_IO_LCDC, 2) ? 16 : 8; // Bit 2 - OBJ (Sprite) Size(0 = 8x8, 1 = 8x16)
	uint8_t LY = mem->read_byte(Memory::ADDR_IO_LY);

	for (int8_t i = current_line_sorted_sprites.size() - 1; i >= 0; i--)
	{
		Sprite sprite = current_line_sorted_sprites[i];

		uint8_t palette = (GET_BIT(sprite.flags, 4)) ?
			mem->read_byte(Memory::ADDR_IO_OBP1) :
			mem->read_byte(Memory::ADDR_IO_OBP0); // Bit4   Palette number  **Non CGB Mode Only** (0 = OBP0, 1 = OBP1)

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
			uint8_t colour_idx;
			if (!flip_x)
				colour_idx = pixel_line[pixel_idx];
			else
				colour_idx = pixel_line[7 - pixel_idx];

			if (colour_idx == 0)
				continue;

			current_line_pixels[sprite.x - 8 + pixel_idx] = get_colour(palette, colour_idx);
		}
	}
}

uint8_t PPU::read_byte_VRAM(uint16_t addr) const
{
	if (mode == Mode::PIXEL_DRAWING)
		return 0xFF;
	else
		return VRAM[addr - Memory::ADDR_VRAM_START];
}

void PPU::write_byte_VRAM(uint16_t addr, uint8_t value)
{
	if (mode == Mode::PIXEL_DRAWING)
		return;
	VRAM[addr - Memory::ADDR_VRAM_START] = value;
}

uint8_t PPU::read_byte_OAM(uint16_t addr) const
{
	if (mode == Mode::OAM_SEARCH || mode == Mode::PIXEL_DRAWING)
		return 0xFF;
	else
		return OAM[addr - Memory::ADDR_OAM_START];
}

void PPU::write_byte_OAM(uint16_t addr, uint8_t value)
{
	if (mode == Mode::OAM_SEARCH || mode == Mode::PIXEL_DRAWING)
		return;
	OAM[addr - Memory::ADDR_OAM_START] = value;
}