// Chip 8 class abstraction
// the size of variables are taken according to the official Chip 8 technical documentation

#include <cstdint>

class chip8
{
private:
    // to store opcode
    uint16_t opcode;

    // memory
    uint8_t memory[4096];

    // general-purpose registers
    uint8_t V[16];

    // additional registers
    uint16_t I;     // index register
    uint16_t pc;    // program counter

    // graphics; monochrome 64x32 pixels screen
    uint8_t gfx[64 * 32];

    // timers
    uint8_t delayTimer;
    uint8_t soundTimer;

    // 16 stack levels and stack pointer on each; sp - to return to the caller.
    uint16_t stack[16];
    uint16_t sp;

    // input keys
    uint8_t key[16];

public:
    chip8() { }
    ~chip8() { }
}

