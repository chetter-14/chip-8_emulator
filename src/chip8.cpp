#include "chip8.h"

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

    // load fontset

    // reset timers
}

void chip8::executeCycle()
{
    // read 2-byte opcode at the address of program counter
    opcode = (memory[pc] << 8) | memory[pc + 1];

}