#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/codegen.h"

static const char *x86_regs[] = { "rax", "rdi", "rsi", "rdx", "rcx", "r8", "r9", "r10", "r11", "r12", "rsp" };

void codegen_x86_64(FILE *out, const Instruction *instrctions, size_t instrCount, targetOS targetArch) {
    fprintf(out, "; Genrated by USM compiler pre-alfa\n\n");
    
    fprintf(out, "section .data\n");
    for(size_t i = 0; i < instrCount; i++) {

        if(instrctions[i].opcode == TOKEN_SECTION && strcmp(instrctions[i].src.val.name, "text") == 0) { break; }
        
        if(instrctions[i].opcode == TOKEN_ID) {
            fprintf(out, "    %s ", instrctions[i].src.val.name);
            switch(instrctions[i].dest.type){
                case OPERAND_IMM:
                    fprintf(out, "dq %d\n", instrctions[i].dest.val.imm);
                    break;
                case OPERAND_VAR:
                    fprintf(out, "db \"%s\", 0\n", instrctions[i].dest.val.name);
            }
        }
    }
    fprintf(out, "\n");

    fprintf(out, "section .text\n");
    fprintf(out, "    default rel\n");
    switch(targetArch){
        case OS_UNIX:
            fprintf(out, "    global _start\n\n");
            break;
        case OS_WINDOWS:
            fprintf(out, "    extern ExitProcess\n");
            fprintf(out, "    global main\n\n");
            break;
    }
    
    for(size_t i = 0; i < instrCount; i++){
        switch(instrctions[i].opcode){
            case TOKEN_ID: continue;

            case TOKEN_LABEL:
                fprintf(out, "%s:\n", instrctions[i].src.val.name);
                break;
            case TOKEN_ADD:
            case TOKEN_SUB:
            case TOKEN_MUL:
            case TOKEN_AND:
            case TOKEN_OR:
            case TOKEN_XOR:
            case TOKEN_SHL:
            case TOKEN_SHR:
            case TOKEN_MOV:{
                const char *cmd = "";
                switch (instrctions[i].opcode) {            // deremens the instruction and wirte it (keep it DRY ladz)
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
                
                fprintf(out, "    %s %s, ", cmd, x86_regs[instrctions[i].dest.val.reg]);
                switch(instrctions[i].src.type){
                    case OPERAND_IMM:
                        fprintf(out, "%d\n", instrctions[i].src.val.imm);
                        break;
                    case OPERAND_REG:
                        fprintf(out, "%s\n", x86_regs[instrctions[i].src.val.reg]);
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
                switch(instrctions[i].opcode){
                    case TOKEN_JMP: cmd = "jmp"; break;
                    case TOKEN_JIE: cmd = "je"; break;
                    case TOKEN_JINE: cmd = "jne"; break;
                    case TOKEN_JIG: cmd = "jg"; break;
                    case TOKEN_JIL: cmd = "jl"; break;
                    case TOKEN_CALL: cmd = "call"; break;
                    default: break;
                }

                fprintf(out, "    %s %s\n", cmd, instrctions[i].src.val.name);
                break;
            }
        case TOKEN_LOAD:
            fprintf(out, "    mov %s, ", x86_regs[instrctions[i].src.val.reg]);
            
            switch(instrctions[i].dest.type){
                case OPERAND_REG:
                    fprintf(out, "[%s]\n", x86_regs[instrctions[i].dest.val.reg]);
                    break;
                case OPERAND_VAR:
                    fprintf(out, "[%s]\n", instrctions[i].dest.val.name);
                    break;
            }
            break;
        case TOKEN_STR:
            fprintf(out, "    mov ");

            switch(instrctions[i].src.type){
                case OPERAND_REG:
                    fprintf(out, "[%s], ", x86_regs[instrctions[i].src.val.reg]);
                    break;
                case OPERAND_VAR:
                    fprintf(out, "[%s], ", instrctions[i].src.val.name);
                break;
            }

            fprintf(out, "%s\n", x86_regs[instrctions[i].dest.val.reg]);
            break;
        case TOKEN_PUSH:
            fprintf(out, "    push ");
            switch(instrctions[i].src.type){
                case OPERAND_REG:
                    fprintf(out, "%s\n", x86_regs[instrctions[i].src.val.reg]);
                    break;
                case OPERAND_IMM:
                    fprintf(out, "%d\n", instrctions[i].src.val.imm);
                    break;
            }
            break;
        case TOKEN_POP:
            fprintf(out, "    pop %s\n", x86_regs[instrctions[i].dest.val.reg]);
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
            const char *destReg = x86_regs[instrctions[i].dest.val.reg];
            if(instrctions[i].dest.val.reg != 0){
                fprintf(out, "; DIV init\n");
                fprintf(out, "    mov rax, %s\n", destReg);
            }
           
               fprintf(out, "    cqo\n");

               switch(instrctions[i].src.type){
                   case OPERAND_REG: {
                       const char *srcReg = x86_regs[instrctions[i].src.val.reg];
                       fprintf(out, "    idiv %s\n", srcReg);
                       break;
                   }
                   case OPERAND_IMM:
                       fprintf(out, "    mov r10, %d\n", instrctions[i].src.val.imm);
                       fprintf(out, "    idiv r10\n");
                    break;
               }
               if(instrctions[i].dest.val.reg != 0) {
                   fprintf(out, "    mov %s, rax\n", destReg);
                   fprintf(out, "; end init\n");
               }
               break;
        }
        case TOKEN_NOT: 
            fprintf(out, "    not %s\n", x86_regs[instrctions[i].dest.val.reg]);
            break;
            
            default: break;
        }
    }
}