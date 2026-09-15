#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/codegen.h"

static const char *x86Regs[] = { "rax", "rdi", "rsi", "rdx", "rcx", "r8", "r9", "r10", "r11", "r12", "rsp" };

void GenX86_64(FILE *out, const Instruction *instructions, size_t instrCount, TargetOS targetArch) {
    fprintf(out, "; Genrated by USM compiler pre-alfa\n\n");

    fprintf(out, "\n");
    
    for(size_t i = 0; i < instrCount; i++){
    
        switch(instructions[i].opcode){

            case TOKEN_SECTION:
                if(strcmp(instructions[i].src.val.name, "data") == 0) { fprintf(out, "section .data\n"); }
                else if (strcmp(instructions[i].src.val.name, "text") == 0) { 
                    fprintf(out, "\nsection .text\n    default rel\n");
                    switch(targetArch){
                        case OS_UNIX:
                            fprintf(out, "    global _start\n\n");
                            break;
                        case OS_WINDOWS:
                            fprintf(out, "    extern ExitProcess\n");
                            fprintf(out, "    global main\n\n");
                            break;
                    }
                }

                
                continue;
            case TOKEN_ID:
                fprintf(out, "    %s ", instructions[i].src.val.name);
                switch(instructions[i].dest.type){
                    case OPERAND_IMM: {
                        const char *size;
                        switch(instructions[i].size){
                            case 1: size = "db"; break;
                            case 2: size = "dw"; break;
                            case 4: size = "dd"; break;
                            case 8: size = "dq"; break;
                            default: size = "dq"; break;
                        }
                        fprintf(out, "%s %lld\n", size, instructions[i].dest.val.imm);
                        break;
                    }
                    case OPERAND_VAR:
                        fprintf(out, "db \"%s\", 0\n", instructions[i].dest.val.name);
                        break;
                }
                continue;
            case TOKEN_LABEL:
                fprintf(out, "%s:\n", instructions[i].src.val.name);
                break;
            case TOKEN_ADD:
            case TOKEN_SUB:
            case TOKEN_MUL:
            case TOKEN_AND:
            case TOKEN_OR:
            case TOKEN_XOR:
            case TOKEN_SHL:
            case TOKEN_SHR:
            case TOKEN_CMP:
            case TOKEN_MOV:{
                const char *cmd = "";
                switch (instructions[i].opcode) {            // deremens the instruction and wirte it (keep it DRY ladz)
                    case TOKEN_MOV: cmd = "mov"; break;
                    case TOKEN_ADD: cmd = "add"; break;
                    case TOKEN_SUB: cmd = "sub"; break;
                    case TOKEN_MUL: cmd = "imul"; break;
                    case TOKEN_AND: cmd = "and"; break;
                    case TOKEN_OR:  cmd = "or";  break;
                    case TOKEN_XOR: cmd = "xor"; break;
                    case TOKEN_CMP: cmd = "cmp"; break;
                    case TOKEN_SHL: cmd = "shl"; break;
                    case TOKEN_SHR: cmd = "shr"; break;
                    default: break;
                }
                
                fprintf(out, "    %s %s, ", cmd, x86Regs[instructions[i].dest.val.reg]);
                switch(instructions[i].src.type){
                    case OPERAND_IMM:
                        fprintf(out, "%lld\n", instructions[i].src.val.imm);
                        break;
                    case OPERAND_REG:
                        fprintf(out, "%s\n", x86Regs[instructions[i].src.val.reg]);
                    break;
                }
                break;
                }
            case TOKEN_JMP:
            case TOKEN_JIE:
            case TOKEN_JINE:
            case TOKEN_JIG:
            case TOKEN_JIL: 
            case TOKEN_CALL: {
                const char *cmd = "";
                switch(instructions[i].opcode){
                    case TOKEN_JMP: cmd = "jmp"; break;
                    case TOKEN_JIE: cmd = "je"; break;
                    case TOKEN_JINE: cmd = "jne"; break;
                    case TOKEN_JIG: cmd = "jg"; break;
                    case TOKEN_JIL: cmd = "jl"; break;
                    case TOKEN_CALL: cmd = "call"; break;
                    default: break;
                }

                fprintf(out, "    %s %s\n", cmd, instructions[i].src.val.name);
                break;
            }
        case TOKEN_LOAD:
            fprintf(out, "    mov %s, ", x86Regs[instructions[i].src.val.reg]);
            
            switch(instructions[i].dest.type){
                case OPERAND_REG:
                    fprintf(out, "[%s]\n", x86Regs[instructions[i].dest.val.reg]);
                    break;
                case OPERAND_VAR:
                    fprintf(out, "[%s]\n", instructions[i].dest.val.name);
                    break;
            }
            break;
        case TOKEN_STR:
            fprintf(out, "    mov ");

            switch(instructions[i].src.type){
                case OPERAND_REG:
                    fprintf(out, "[%s], ", x86Regs[instructions[i].src.val.reg]);
                    break;
                case OPERAND_VAR:
                    fprintf(out, "[%s], ", instructions[i].src.val.name);
                break;
            }

            fprintf(out, "%s\n", x86Regs[instructions[i].dest.val.reg]);
            break;
        case TOKEN_PUSH:
            fprintf(out, "    push ");
            switch(instructions[i].src.type){
                case OPERAND_REG:
                    fprintf(out, "%s\n", x86Regs[instructions[i].src.val.reg]);
                    break;
                case OPERAND_IMM:
                    fprintf(out, "%lld\n", instructions[i].src.val.imm);
                    break;
            }
            break;
        case TOKEN_POP:
            fprintf(out, "    pop %s\n", x86Regs[instructions[i].dest.val.reg]);
            break;
        case TOKEN_EXIT:
            fprintf(out, "; EXIT\n");
            switch(targetArch){
                case OS_UNIX:
                    fprintf(out, "    mov rax, 60\n");
                    fprintf(out, "    mov rdi, 0\n");
                    fprintf(out, "    syscall\n");
                    break;
                case OS_WINDOWS:
                    fprintf(out, "    mov rcx, 0\n");
                    fprintf(out, "    call ExitProcess\n");
                    break;
            }
            break;
            
        case TOKEN_RET:
            fprintf(out, "    ret\n");
            break; 
        case TOKEN_SYSCALL:
            fprintf(out, "    syscall\n");
            break;
        case TOKEN_DIV: {
            const char *destReg = x86Regs[instructions[i].dest.val.reg];
            if(instructions[i].dest.val.reg != 0){
                fprintf(out, "; DIV init\n");
                fprintf(out, "    mov rax, %s\n", destReg);
            }
           
               fprintf(out, "    cqo\n");

               switch(instructions[i].src.type){
                   case OPERAND_REG: {
                       const char *srcReg = x86Regs[instructions[i].src.val.reg];
                       fprintf(out, "    idiv %s\n", srcReg);
                       break;
                   }
                   case OPERAND_IMM:
                       fprintf(out, "    mov r10, %lld\n", instructions[i].src.val.imm);
                       fprintf(out, "    idiv r10\n");
                    break;
               }
               if(instructions[i].dest.val.reg != 0) {
                   fprintf(out, "    mov %s, rax\n", destReg);
                   fprintf(out, "; end init\n");
               }
               break;
        }
        case TOKEN_NOT: 
            fprintf(out, "    not %s\n", x86Regs[instructions[i].dest.val.reg]);
            break;
            
            default: break;
        }
    }
}