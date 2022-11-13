#include "chip8.h"
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <ctime>

// Initialize registers, timers, memory, etc.
void chip8::initialize()
{
    pc = 0x0200;    // program is placed at 0x0200 
    opcode = 0;
    I = 0;
    stackLevel = 0;

    // clear display
    for (int i = 0; i < GRAPHICS_PIXEL_RESOL; i++)
        displayScreen[i] = 0;
    
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
    srand(time(0));

    // read 2-byte opcode at the address of program counter
    opcode = (memory[pc] << 8) | memory[pc + 1];

    // initially look at the first 4 bits of opcode
    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode & 0x00FF) 
            {
                case 0x00E0:    // 0x00E0 clears the screen
                    for (int i = 0; i < GRAPHICS_PIXEL_RESOL; i++)
                        displayScreen[i] = 0;
                    pc += 2;
                    break;
                
                case 0x00EE:    // 0x00EE returns from a subroutine
                    pc = stack[--stackLevel];
                    break;

                default:        // 0x0NNN call machine code routine at address NNN
                    stack[stackLevel++] = pc + 2;
                    pc = opcode & 0x0FFF;
                    break;
            }
            break;

        case 0x1000:            // 0x1NNN JUMPS to address NNN
            pc = opcode & 0x0FFF;
            break;

        case 0x2000:            // 0x2NNN CALLS a subroutine at NNN
            stack[stackLevel++] = pc + 2;
            pc = opcode & 0x0FFF;
            break;

        case 0x3000:            // 0x3XNN skips the next instruction if VX == NN
            uint8_t regNumber = (opcode & 0x0F00) >> 8;
            uint8_t numberToCompare = opcode & 0x00FF;
            if ( V[regNumber] == numberToCompare )
                pc += 2;
            pc += 2;
            break;

        case 0x4000:            // 0x4XNN skips the next instruction if VX != NN
            uint8_t regNumber = (opcode & 0x0F00) >> 8;
            uint8_t numberToCompare = opcode & 0x00FF;
            if ( V[regNumber] != numberToCompare )
                pc += 2;
            pc += 2;
            break;

        case 0x5000:            // 0x5XY0 skips the next instruction if VX != VY
            uint8_t regNumberX = (opcode & 0x0F00) >> 8;
            uint8_t regNumberY = (opcode & 0x00F0) >> 4;
            if ( V[regNumberX] != V[regNumberY] )
                pc += 2;
            pc += 2;
            break;

        case 0x6000:            // 0x6XNN sets VX to NN
            uint8_t regNumber = (opcode & 0x0F00) >> 8;
            V[regNumber] = opcode & 0x00FF;
            pc += 2;
            break;

        case 0x7000:            // 0x7XNN adds NN to VX (carry flag is not set)
            uint8_t regNumber = (opcode & 0x0F00) >> 8;
            V[regNumber] = V[regNumber] + opcode & 0x00FF;
            pc += 2;
            break;

        case 0x8000:
            switch (opcode & 0x000F) 
            {
                case 0x0000:    // 0x8XY0 sets VX to the value of VY
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    uint8_t regNumberY = (opcode & 0x00F0) >> 4;
                    V[regNumberX] = V[regNumberY];
                    pc += 2;
                    break;
                
                case 0x0001:    // 0x8XY1 sets VX to VX or VY (bitwise OR)
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    uint8_t regNumberY = (opcode & 0x00F0) >> 4;
                    V[regNumberX] |= V[regNumberY];
                    pc += 2;
                    break;

                case 0x0002:    // 0x8XY2 sets VX to VX and VY (bitwise AND)
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    uint8_t regNumberY = (opcode & 0x00F0) >> 4;
                    V[regNumberX] &= V[regNumberY];
                    pc += 2;
                    break;

                case 0x0003:    // 0x8XY3 sets VX to VX xor VY (bitwise XOR)
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    uint8_t regNumberY = (opcode & 0x00F0) >> 4;
                    V[regNumberX] ^= V[regNumberY];
                    pc += 2;
                    break;
                
                case 0x0004:    // 0x8XY4 adds VY to VX (carry flag is set)
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    uint8_t regNumberY = (opcode & 0x00F0) >> 4;
                    if ( V[regNumberX] > (0xFF - V[regNumberY]) )
                        V[0xF] = 1;
                    else 
                        V[0xF] = 0;
                    V[regNumberX] += V[regNumberY];
                    pc += 2;
                    break;

                case 0x0005:    // 0x8XY5 VY is subtracted from VX (with "no borrow" flag)
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    uint8_t regNumberY = (opcode & 0x00F0) >> 4;
                    if ( V[regNumberX] < V[regNumberY] )
                        V[0xF] = 0;
                    else
                        V[0xF] = 1;
                    V[regNumberX] -= V[regNumberY];
                    pc += 2;
                    break;

                case 0x0006:    // 0x8XY6 stores the least significant bit of VX in VF and then shifts VX to the right by 1
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    V[0xF] = V[regNumberX] & 0x01;
                    V[regNumberX] >>= 1;
                    pc += 2;
                    break;

                case 0x0007:    // 0x8XY7 sets VX to VY minus VX (with "no borrow" flag)
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    uint8_t regNumberY = (opcode & 0x00F0) >> 4;
                    if ( V[regNumberY] < V[regNumberX] )
                        V[0xF] = 0;
                    else
                        V[0xF] = 1;
                    V[regNumberX] = V[regNumberY] - V[regNumberX];
                    pc += 2;
                    break;

                case 0x000E:    // 0x8XYE stores the most significant bit of VX in VF and then shifts VX to the left by 1
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    V[0xF] = (V[regNumberX] & 0x80) >> 7;
                    V[regNumberX] <<= 1;
                    pc += 2;
                    break;
                
                default:
                    printf("Unknown opcode: 0x%04X\n", opcode);
            }
            break;

        case 0x9000:            // 0x9XY0 skips the next instruction if VX does not equal VY
            uint8_t regNumberX = (opcode & 0x0F00) >> 8;
            uint8_t regNumberY = (opcode & 0x00F0) >> 4;
            if ( V[regNumberX] != V[regNumberY] )
                pc += 2;
            pc += 2;
            break;

        case 0xA000:            // 0xANNN sets I to the address NNN
            I = opcode & 0x0FFF;
            pc += 2;
            break;

        case 0xB000:            // 0xBNNN jumps to the address NNN plus V0
            pc = (opcode & 0x0FFF) + V[0];
            break;

        case 0xC000:            // 0xCXNN sets VX to the result of a bitwise AND operation on a random number (Typically: 0 to 255) and NN
            uint8_t regNumberX = (opcode & 0x0F00) >> 8;
            V[regNumberX] = (opcode & 0x00FF) & (uint8_t)(rand() % 255);
            pc += 2;
            break;
        
        case 0xD000:            // 0xDXYN draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
            uint8_t regNumberX = (opcode & 0x0F00) >> 8;
            uint8_t regNumberY = (opcode & 0x00F0) >> 4;

            uint8_t x = V[regNumberX];
            uint8_t y = V[regNumberY];

            uint8_t height = opcode & 0x000F;
            uint8_t pixel;  // one row of 8 pixels

            V[0xF] = 0;
            for (int yLine = 0; yLine < height; yLine++)
            {
                pixel = memory[I + yLine];
                for (int xLine = 0; xLine < 8; xLine++)
                {
                    if ( (pixel & (0x80 >> xLine) ) != 0)
                    {
                        if (displayScreen[ ( (y + yLine) * DISPLAY_HEIGHT_PIXELS + x + xLine) % GRAPHICS_PIXEL_RESOL] == 1)
                            V[0xF] = 1;
                        displayScreen[ ( (y + yLine) * DISPLAY_HEIGHT_PIXELS + x + xLine) % GRAPHICS_PIXEL_RESOL] ^= 1 /* (pixel & (0x80 >> xLine) ) */;
                    }
                }
            }

            drawFlag = true;
            pc += 2;
            break;

        case 0xE000:
            switch (opcode & 0x00F0) 
            {
                case 0x0090:    // 0xEX9E skips the next instruction if the key stored in VX is pressed
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    if (key[ V[regNumberX] ] != 0)
                        pc += 4;
                    else
                        pc += 2;
                    break;

                case 0x00A0:    // 0xEXA1 skips the next instruction if the key stored in VX is not pressed
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    if (key[ V[regNumberX] ] == 0)
                        pc += 4;
                    else
                        pc += 2;
                    break;
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FF)
            {
                case 0x0007:    // 0xFX07 sets VX to the value of the delay timer
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    V[regNumberX] = delayTimer;
                    pc += 2;
                    break;

                case 0x000A:    // 0xFX0A a key press is awaited, and then stored in VX
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i < KEYS_NUMBER; i++)
                    {
                        if (key[i] != 0)
                        {
                            V[regNumberX] = i;
                            break;
                        }
                        if (i == KEYS_NUMBER - 1)
                            i = -1;
                    }
                    break;

                case 0x0015:    // 0xFX15 sets the delay timer to VX
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    delayTimer = V[regNumberX];
                    pc += 2;
                    break;

                case 0x0018:    // 0xFX18 sets the sound timer to VX
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    soundTimer = V[regNumberX];
                    pc += 2;
                    break;
                
                case 0x001E:    // 0xFX1E adds VX to I (no carry flag)
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    I += V[regNumberX];
                    pc += 2;
                    break;
                
                case 0x0029:    // 0xFX29 sets I to the location of the sprite for the character in VX
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    I = V[regNumberX] * 0x5;    // didn't get it
                    pc += 2;
                    break;

                case 0x0033:    // 0xFX33 stores the binary-coded decimal representation of VX
                    uint8_t regNumberX = (opcode & 0x0F00) >> 8;
                    memory[I + 0] = V[regNumberX] / 100;
                    memory[I + 1] = V[regNumberX] / 10 % 10;
                    memory[I + 2] = V[regNumberX] % 10;
                    pc += 2;
                    break;

                case 0x0055:    // 0xFX55 stores from V0 to VX (including VX) in memory, starting at address I
                    int8_t regNumberX = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i <= regNumberX; i++)
                    {
                        memory[I + i] = V[i];
                    }
                    pc += 2;
                    break;

                case 0x0065:    // 0xFX65 fills from V0 to VX (including VX) with values from memory, starting at address I
                    int8_t regNumberX = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i <= regNumberX; i++)
                    {
                        V[i] = memory[I + i];
                    }
                    pc += 2;
                    break;
            }
            break;

        default:
            printf("Unknown opcode: 0x%04X\n", opcode);
    }

    // update timers
    if (delayTimer > 0)
        delayTimer--;

    if (soundTimer > 0)
    {
        if (soundTimer == 1)
            printf("BEEP!\n");
        soundTimer--;
    }

}