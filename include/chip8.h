// Chip 8 class abstraction
// the size of variables are taken according to the official Chip 8 technical documentation

#include <cstdint>

#define MEMORY_SIZE 4096
#define REGS_NUMBER 16
#define GRAPHICS_PIXEL_RESOL 64 * 32
#define STACK_LEVELS 16
#define KEYS_NUMBER 16

// uint8_t chip8_fontset[80] = 
// {
//     0xF0, 0x90, 0x90, 0x90, 0xF0,
//     0x20, 0x60, 0x20, 0x20, 0x70,

// };


class chip8
{
private:
    // to store opcode
    uint16_t opcode;

    // memory
    uint8_t memory[MEMORY_SIZE];

    // general-purpose registers
    uint8_t V[REGS_NUMBER];

    // additional registers
    uint16_t I;     // index register
    uint16_t pc;    // program counter

    // graphics; monochrome 64x32 pixels screen
    uint8_t gfx[GRAPHICS_PIXEL_RESOL];

    // timers
    uint8_t delayTimer;
    uint8_t soundTimer;

    // 16 stack levels and stack pointer on each; sp - to return to the caller.
    uint16_t stack[STACK_LEVELS];
    uint16_t sp;

    // input keys
    uint8_t key[KEYS_NUMBER];

public:
    chip8() { }
    ~chip8() { }

    void initialize();
    void executeCycle();
};
