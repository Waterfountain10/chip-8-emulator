//
// Created by albert on 5/16/25.
//

#include "chip8.h"
#include <iostream>
#include <fstream>
#include <format>
#include <memory>
#include <sstream>

// This is all the implementation for the chip-8 hardware
namespace Chip8 {
    Chip8::Chip8() :    // initialize list
    registers(std::make_unique<std::array<uint8_t, 16>>()),
    memory(std::make_unique<std::array<uint8_t, 4096>>()),
    stack(std::make_unique<std::array<uint16_t, 16>>()),
    gfx(std::make_shared<std::array<uint8_t, 64 * 32>>()),
    fonts {{
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    }}
    {
        this->init_counters();
        this->init_timers(0, 0);    // use default argument values 0, 0
    }

    int Chip8::init_counters() {
        this->index_reg = 0x000; // also known as I  (used to store memory addresses)
        this->program_ctr = 0x000;
        this->stack_ptr = 0x000;    // init to final memory addr

        return 0;
    }

    int Chip8::init_timers(uint8_t delay_time = 0, uint8_t sound_time = 0) {
        this->delay_timer = delay_time;
        this->sound_timer = sound_time;

        return 0;
    }

    int Chip8::load_rom(std::ifstream *file_stream) {
        std::streamsize file_size = file_stream->tellg();
        std::vector<char> buffer(file_size);

        file_stream->seekg(0, std::ios::beg);
        if (!file_stream->read(buffer.data(), file_size))
            throw std::runtime_error("Read failed");

        std::copy(
            buffer.begin(),
            buffer.end(),
            memory->begin() + rom_start_addr
        );

        return 0;
    }

    bool Chip8::load_fonts_in_memory(std::string start_address_hex) {
        // Stores 5 bytes * 16 fonts = 80 bytes inside [0x050, 0x09F]
        int start_address_int;
        std::stringstream ss;
        ss << std::hex << start_address_hex;
        ss >> start_address_int;
        if (start_address_int > 432) {
            std::cerr << "start_address is too high to fit in between 0x000 and 0x200";
            return false;
        }
        for (int i = 0; i < 80; i++) {
            (*this->memory)[start_address_int + i] = this->fonts[i];
        }
        return true;
    }
}