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

    // initially look at the first 4 bits of opcode
    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode & 0x00FF) 
            {
                case 0x00E0:    // 0x00E0 clears the screen

                    break;
                
                case 0x00EE:    // 0x00EE returns from a subroutine

                    break;

                default:        // 0x0NNN call machine code routine at address NNN

            }
            break;

        case 0x1000:            // 0x1NNN jumps to address NNN

            break;

        case 0x2000:            // 0x2NNN calls a subroutine at NNN

            break;

        case 0x3000:            // 0x3XNN skips the next instruction if VX == NN

            break;

        case 0x4000:            // 0x4XNN skips the next instruction if VX != NN

            break;

        case 0x5000:            // 0x5XY0 skips the next instruction if VX != VY

            break;

        case 0x6000:            // 0x6XNN sets VX to NN

            break;

        case 0x7000:            // 0x7XNN adds NN to VX (carry flag is not set)

            break;

        case 0x8000:
            switch (opcode & 0x000F) 
            {
                case 0x0000:    // 0x8XY0 sets VX to the value of VY

                    break;
                
                case 0x0001:    // 0x8XY1 sets VX to VX or VY (bitwise OR)

                    break;

                case 0x0002:    // 0x8XY2 sets VX to VX and VY (bitwise AND)

                    break;

                case 0x0003:    // 0x8XY3 sets VX to VX xor VY (bitwise XOR)

                    break;
                
                case 0x0004:    // 0x8XY4 adds VY to VX (carry flag is set)

                    break;

                case 0x0005:    // 0x8XY5 VY is subtracted from VX (with flag)

                    break;

                case 0x0006:    // 0x8XY6 stores the least significant bit of VX in VF and then shifts VX to the right by 1

                    break;

                case 0x0007:    // 0x8XY7 sets VX to VY minus VX (with flag)

                    break;

                case 0x000E:    // 0x8XYE stores the most significant bit of VX in VF and then shifts VX to the left by 1

                    break;
                
                default:
                    printf("Unknown opcode: 0x%04X\n", opcode);
            }
            break;

        case 0x9000:            // 0x9XY0 skips the next instruction if VX does not equal VY

            break;

        case 0xA000:            // 0xANNN sets I to the address NNN
            I = opcode & 0x0FFF;
            pc += 2;
            break;

        case 0xB000:            // 0xBNNN jumps to the address NNN plus V0

            break;

        case 0xC000:            // 0xCXNN sets VX to the result of a bitwise AND operation on a random number (Typically: 0 to 255) and NN

            break;
        
        case 0xD000:            // 0xDXYN draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels

            break;

        case 0xE000:
            switch (opcode & 0x00F0) 
            {
                case 0x0090:    // 0xEX9E skips the next instruction if the key stored in VX is pressed

                    break;

                case 0x00A0:    // 0xEXA1 skips the next instruction if the key stored in VX is not pressed

                    break;
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FF)
            {
                case 0x0007:    // 0xFX07 sets VX to the value of the delay timer

                    break;

                case 0x000A:    // 0xFX0A a key press is awaited, and then stored in VX

                    break;

                case 0x0015:    // 0xFX15 sets the delay timer to VX

                    break;

                case 0x0018:    // 0xFX18 sets the sound timer to VX

                    break;
                
                case 0x001E:    // 0xFX1E adds VX to I (no carry flag)

                    break;
                
                case 0x0029:    // 0xFX29 sets I to the location of the sprite for the character in VX

                    break;

                case 0x0033:    // 0xFX33 stores the binary-coded decimal representation of VX

                    break;

                case 0x0055:    // 0xFX55 stores from V0 to VX (including VX) in memory, starting at address I

                    break;

                case 0x0065:    // 0xFX65 fills from V0 to VX (including VX) with values from memory, starting at address I

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