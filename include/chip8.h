// Chip 8 class abstraction
// the size of variables are taken according to the official Chip 8 technical documentation

#include <cstdint>

#define FONTSET_SIZE 80
#define MEMORY_SIZE 4096
#define REGS_NUMBER 16
#define GRAPHICS_PIXEL_RESOL 64 * 32
#define DISPLAY_HEIGHT_PIXELS 64
#define STACK_LEVELS 16
#define KEYS_NUMBER 16


class chip8
{
private:

    uint8_t fontset[FONTSET_SIZE] = 
    {
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
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // to store opcode
    uint16_t opcode;

    // memory
    uint8_t memory[MEMORY_SIZE];

    // general-purpose registers
    uint8_t V[REGS_NUMBER];

    // additional registers
    uint16_t I;     // index register
    uint16_t pc;    // program counter

    // timers
    uint8_t delayTimer;
    uint8_t soundTimer;

    // 16 stack levels and each stores an address to return to; stackLevel - on which level of stack we are now.
    uint16_t stack[STACK_LEVELS];
    uint8_t stackLevel;

public:
    bool drawFlag;

    // graphics; monochrome 64x32 pixels screen
    uint8_t displayScreen[GRAPHICS_PIXEL_RESOL];
    
    // input keys
    uint8_t key[KEYS_NUMBER];

public:
    chip8() { }
    ~chip8() { }

    void initialize();
    bool loadGame(const char* gameFileName);
    void executeCycle();
};
