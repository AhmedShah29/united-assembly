![Testing Succeed](https://img.shields.io/badge/Testing-Succeed-brightgreen?style=for-the-badge&labelColor=000000&logo=check-circle&logoColor=white)

# United Assembly

United Assembly (USM) is an educational project designed to demystify the core concepts of Assembly language. Instead of getting bogged down by the complex, architecture-specific syntax of x86, ARM, or RISC-V, USM provides a unified and simplified syntax.

### Project Goal

The sole purpose of USM is to teach low-level programming fundamentals: **registers, memory, the stack, and control flow**. It bridges the gap between abstract concepts and real CPU architectures (x64, ARM, RISC-V).


## Target Architectures

**Note about the x32 architectures:**
Due to usm having a 10 rigesters and some of the x32 CPUs dont have 10 rigesters usm Supports:

### Work in Progress

- **x86_64(AMD/Intel 64-bit)**

### Coming

- **ARM64/32**
- **RISC-V 64/32**
  
## Instruction Set Reference (ISR)

### Data Movement & Memory Access
**(Uses `[ ]` for address)**

| Instruction | Syntax | Description |
| :--- | :--- | :--- |
| MOV | `MOV REG/IMM, REG` | Copies the value from the source (register or immediate value) into the destination register. |
| LOAD | `LOAD [VAR], REG` | Loads the value from the memory address into the register. |
| STR | `STR REG, [VAR]` | Stores the value from the register into the memory address. |

### Arithmetic Operations

| Instruction | Syntax | Description |
| :--- | :--- | :--- |
| ADD | `ADD REG, REG/IMM` | Adds the source value to the destination register and stores the result in the destination. |
| SUB | `SUB REG, REG/IMM` | Subtracts the source value from the destination register and stores the result in the destination. |
| MUL | `MUL REG, REG/IMM` | Multiplies the destination register by the source value and stores the result in the destination. |
| DIV | `DIV REG, REG/IMM` | Divides the destination register by the source value and stores the quotient (result) in the destination. |

### Logical & Bitwise

| Instruction | Syntax | Description |
| :--- | :--- | :--- |
| AND | `AND REG, REG/IMM` | Performs a bitwise AND operation. |
| OR | `OR REG, REG/IMM` | Performs a bitwise OR operation. |
| XOR | `XOR REG, REG/IMM` | Performs a bitwise XOR operation. |
| SHL | `SHL REG, REG/IMM` | Shifts the bits of the register to the left. |
| SHR | `SHR REG, REG/IMM` | Shifts the bits of the register to the right. |
| NOT | `NOT REG` | Performs a bitwise NOT (inversion) on the register. |

### Control Flow & Branching

| Instruction | Syntax | Description |
| :--- | :--- | :--- |
| CMP | `CMP REG, REG/IMM` | Compares the first operand with the second and sets flags for branching. |
| JMP | `JMP LABEL` | Unconditional jump to the specified label/address. |
| JIE | `JIE LABEL` | Jump if equal (if operands in CMP are equal). |
| JINE | `JINE LABEL` | Jump if not equal. |
| JIG | `JIG LABEL` | Jump if greater (if the first operand is larger than the second). |
| JIL | `JIL LABEL` | Jump if less (if the first operand is smaller than the second). |

### Stack & System

| Instruction | Syntax | Description |
| :--- | :--- | :--- |
| PUSH | `PUSH REG/IMM` | Pushes a value onto the top of the stack. |
| POP | `POP REG` | Pops a value from the top of the stack and places it into the register. |
| CALL | `CALL LABEL` | Calls a function (pushes return address, jumps to label). |
| RET | `RET` | Returns from a function (pops return address, jumps back). |
| SYSCALL | `SYSCALL` | Invokes an OS system call (expects args in registers). |
| EXIT | `EXIT` | Safely terminates program execution using the exit code stored in R0. |
