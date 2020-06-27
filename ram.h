/*
    Ben Eater's 8-bit Computer Emulator. Made by Rubem Nobre @ https://github.com/rubemnobre/eater-emulator
    Copyright (C) 2020 Rubem Nobre

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef RAM_H
#define RAM_H
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace RAM {
    constexpr int RAM_SIZE = 16;

    std::array<std::uint8_t, RAM_SIZE> loadFromFile(FILE *ramFile) {
        std::array<std::uint8_t, RAM_SIZE> ram;
        for (auto & r : ram) {
            fread(&r, sizeof(std::uint8_t), 1, ramFile);
        }
        return ram;
    }

    std::array<std::uint8_t, RAM_SIZE> loadNull() {
        std::array<std::uint8_t, RAM_SIZE> ram;
        for(auto & r : ram) {
            r = 0;
        }
        return ram;
    }

    void dump(const std::array<std::uint8_t, RAM_SIZE> &ram) {
        FILE *dump = fopen("dump.ram", "wb");
        for(const auto & r : ram) {
            fputc(r, dump);
        }
        fclose(dump);
    }
}
#endif
