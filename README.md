# United Assembly

United Assembly is a project to make a unite most of the assembly features in a 1 language that transplies/compiles to that target choose of the devloper sutch as **x32 x64 x86 Intel/AMD CPUs** from the same United Assembly(usm) code

## Target Architectures

Note about the x32 architectures:
> Due to usm having a 10 rigesters and some of the x32 CPUs dont have 10 rigesters usm Supports:

- **x64(AMD/Intel 64-bit)**
- **ARM64/32**
- **RISC-V 64/32** (as a future mybe)
  
## Instruction Set Reference (ISR)

### Data Movement

| Instruction | Syntax | Description |
| :--- | :--- | :--- |
| MOV | `MOV REG, REG/IMM` | Copies the value from the source (register or immediate value) into the destination register. |

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

### Memory Access
*(Uses `[ ]` for address)*

| Instruction | Syntax | Description |
| :--- | :--- | :--- |
| LOAD | `LOAD [VAR], REG` | Loads the value from the memory address into the register. |
| STR | `STR REG, [VAR]` | Stores the value from the register into the memory address. |

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
| EXIT | `EXIT` | Safely terminates program execution using the exit code stored in R0. |
