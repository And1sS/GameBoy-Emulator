#include "CppUnitTest.h"
#include "../GameBoy_Emulator/CPU.h"
#include "../GameBoy_Emulator/CPU.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	constexpr uint8_t LDA = 0x3e;
	constexpr uint8_t STA = 0xea;
	constexpr uint8_t LDB = 0x06;
	constexpr uint8_t ADD_A_B = 0x80;
	constexpr uint8_t SUB_A_B = 0x90;
	constexpr uint8_t DAA = 0x27;

	TEST_CLASS(UnitTest)
	{
		
	public:
		

		TEST_METHOD(simple_add)
		{
			Memory* mem = new Memory;
			mem->write_bytes(0,
				{
						LDA, 15, LDB, 6, ADD_A_B, STA, 0x00, 0x01
				});

			CPU cpu(mem);
			for (size_t i = 0; i < 4; i++)
				cpu.execute_instruction();

			Assert::AreEqual(mem->read_byte(0x0100), (uint8_t)21);
		}
		
		TEST_METHOD(daa_instruction)
		{
			auto to_bcd = [](int a) -> uint8_t { return ((a / 10) << 4) + (a % 10); };

			Memory* mem = new Memory;
			for (size_t i = 0; i < 100; i++)
				for (size_t j = 0; j < 100; j++)
				{
					mem->write_bytes(0,
						{
								LDA, to_bcd(i), LDB, to_bcd(j), ADD_A_B, DAA, STA, 0x00, 0x01
						});
					CPU cpu(mem);
					for (size_t i = 0; i < 5; i++)
						cpu.execute_instruction();

					Assert::AreEqual(mem->read_byte(0x0100), to_bcd((i+j) % 100));


					mem->write_bytes(0,
						{
								LDA, to_bcd(i), LDB, to_bcd(j), SUB_A_B, DAA, STA, 0x00, 0x01
						});
					CPU cpu2(mem);
					for (size_t i = 0; i < 5; i++)
						cpu2.execute_instruction();

					Assert::AreEqual(mem->read_byte(0x0100), to_bcd((100 + i - j) % 100));
				}


		}
	};
}
