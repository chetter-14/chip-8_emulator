#include "chip8.h"
#include <cstdio>
#include <vector>

// Initialize registers, timers, memory, etc.
void chip8::initialize()
{
    pc = 0x0200;    // program is placed at 0x0200 
    opcode = 0;
    I = 0;
    sp = 0;

    // clear display
    for (int i = 0; i < GRAPHICS_PIXEL_RESOL; i++)
        gfx[i] = 0;
    
    // clear stack array
    for (int i = 0; i < STACK_LEVELS; i++)
        stack[i] = 0;

    // clear registers
    for (int i = 0; i < REGS_NUMBER; i++)
        V[i] = 0;

    // clear memory 
    for (int i = 0; i < MEMORY_SIZE; i++)
        memory[i] = 0;

    // load fontset (in memory: 0x0000 - 0x0050)
    for (int i = 0; i < FONTSET_SIZE; i++)
        memory[i] = fontset[i];

    // reset timers
}

void chip8::loadGame(const char* gameFileName)
{
    FILE* fp = fopen(gameFileName, "rb");
    
    // get the size of file
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    
    std::vector<uint8_t> buffer(1024);   // we suggest the size of the program is 1 kB 
    std::fread(&buffer[0], sizeof(buffer[0]), size, fp);

    for (int i = 0; i < size; i++)
        memory[i + 0x0200] = buffer[i];

    
}

void chip8::executeCycle()
{
    // read 2-byte opcode at the address of program counter
    opcode = (memory[pc] << 8) | memory[pc + 1];

}