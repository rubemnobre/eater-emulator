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
