/*
 * Description:
 * This program reads assembly-like instructions from standard input and
 * translates them into 16-bit hexadecimal machine code. It supports a small
 * instruction set, parses registers and immediate values, and encodes each
 * instruction based on its format and operand types.
 *
 * Author: Griffin Shay
 * Date: 9/18/2024
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INSTRUCTIONS 1024

// Structure representing an instruction definiton.
typedef struct 
{
    const char *opcode;
    unsigned int hex;
    int numOperands;
} Instruction;

// Table of supported instructioons.
Instruction Instructions[] = {
    {"ADD", 0x0, 3},
    {"SUB", 0x1, 3},
    {"MUL", 0x2, 3},
    {"DIV", 0x3, 3},
    {"AND", 0x4, 3},
    {"ORR", 0x5, 3},
    {"NOT", 0x6, 2},
    {"SHL", 0x7, 3},
    {"SHR", 0x8, 3},
    {"CMP", 0x9, 2},
    {"MOV", 0xA, 2},
    {"LDR", 0xB, 2},
    {"STR", 0xC, 2},
    {"NOP", 0xD, 0},
    {"JMP", 0xE, 1},
    {"JEQ", 0xF, 1},
};

// Returns the index of the opcode in the instruction table.
int GetOpcode(const char *opcode)
{
    for(int i = 0; i < 16; i++)
    {
        if(strcmp(Instructions[i].opcode, opcode) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Parses a register string and returns its numeric value.
int Register(const char *reg)
{
    if(reg[0] == 'R')
    {
        char *end;
        int regNum = strtol(&reg[1], &end, 10);

        if(*end == '\0' && regNum >= 0 && regNum <= 15)
        {
            return regNum;
        }
    }
    return -1;
}

// Applies a bitmask to fit a value into a given number of bits.
int TwosCompliment(int value, int bits)
{
    int shift = (1 << bits) - 1;
    return value & shift;
}

// Parses and encodes a single instruction line.
void ProcessInstructions(char *line)
{
    char op[10];
    char operand1[10], operand2[10], operand3[10];
    int operands[3] = {0, 0, 0};

    // Parse opcode and up to 3 operands.
    int lineCode = sscanf(line, "%s %s %s %s", op, operand1, operand2, operand3);
    int opcodeIndex = GetOpcode(op);
    
    if(opcodeIndex < 0)
    {
        return;
    }

    Instruction inst = Instructions[opcodeIndex];

    // Start building instruction (opcode in upper 4 bits).
    unsigned int instruction = inst.hex << 12;

    int rd = -1, rs1 = -1, rs2 = -1, imm = 0;

    if(inst.numOperands >= 1 && lineCode >= 2)
    {
        rd = Register(operand1);
    }
    if(inst.numOperands >= 2 && lineCode >= 3)
    {
        if(operand2[0] == 'R')
        {
            rs1 = Register(operand2);
        }
        else
        {
            imm = strtol(operand2, NULL, 0);
        }
    }
    if(inst.numOperands >= 3 && lineCode >= 4)
    {
        rs2 = Register(operand3);
    }
    
    // Encode instruction based on opcode type. 
    if(strcmp(op, "ADD") == 0 || strcmp(op, "SUB") == 0 || 
        strcmp(op, "MUL") == 0 || strcmp(op, "DIV") == 0 || 
        strcmp(op, "AND") == 0 || strcmp(op, "ORR") == 0 ||
        strcmp(op, "SHL") == 0 || strcmp(op, "SHR") == 0)
    {
        instruction |= (rd << 8) | (rs1 << 4) | rs2;
    }
    else if(strcmp(op, "CMP") == 0)
    {
        instruction |= (rd << 8) | (rs1 << 4);
    }
    else if(strcmp(op, "MOV") == 0)
    {
        if(operand2[0] == 'R')
        {
            instruction |= (rd << 8) | (rs1 << 4);
        }
        else
        {
            instruction |= (rd << 8) | (imm & 0xFF);
        }
    }
    else if(strcmp(op, "NOT") == 0)
    {
        instruction |= (rd << 8) | (rs1 << 4);
    }
    else if(strcmp(op, "STR") == 0)
    {
        if(operand2[0] == 'R')
        {
            instruction |= (rd << 8) | (rs1 & 0xF);
        }
        else
        {
            instruction |= (rd << 8) | (imm & 0xFF);
        }
    }
    else if(strcmp(op, "LDR") == 0)
    {
        if(operand2[0] == 'R')
        {
            instruction |= (rd << 8) | (rs1 & 0xF);
        }
        else
        {
            instruction |= (rd << 8) | (imm & 0xFF);
        }
    }
    else if(strcmp(op, "JMP") == 0 || strcmp(op, "JEQ") == 0)
    {
        instruction |= (strtol(operand1, NULL, 0) & 0xFFF);
    }
    else if(strcmp(op, "NOP") == 0)
    {
        instruction |= 0;
    }

    // Output encoded instruction in hex.
    printf("%04X", instruction);
}

// Reads input lines and processes each instruction.
void assemble()
{
    char line[256];
    int lineCount = 0;

    while(fgets(line, sizeof(line), stdin) && lineCount < MAX_INSTRUCTIONS)
    {
        char *pos = line;
        while(isspace(*pos))
        {
            pos++;
        }
        if(*pos == '\0' || *pos == '#')
        {
            continue;
        }

        ProcessInstructions(pos);
        lineCount++;
    }
}

int main(void)
{
    assemble();
    return 0;
}
