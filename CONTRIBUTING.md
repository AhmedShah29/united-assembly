# Contributing to United Assembly (USM)

Thank you for your interest in contributing to the USM Compiler! This document outlines the guidelines, workflow, and coding standards to keep the codebase clean, consistent, and educational.

## Getting Started & Workflow

1. **Clone** the repository:
   ```bash
   git clone https://github.com/AhmedShah29/united-assembly.git
   cd united-assembly
   ```
2. Make your changes and ensure the project builds successfully.
3. Open a **Pull Request** against the `main` branch with a clear description of your changes.

---

## Coding Standards & Naming Conventions

To maintain readability, please strictly follow these naming rules:

| Category | Convention | Example |
| :--- | :--- | :--- |
| **Types** (Structs, Enums, Typedefs) | `PascalCase` | `TokenType`, `Instruction`, `TargetOS` |
| **Variables** (Local, Global, Members) | `camelCase` | `currentLine`, `instrCount`, `fileSize` |
| **Helper Functions** (Internal/Static) | `snake_case` | `parse_immediate`, `get_token_enum`, `hash_string` |
| **Main/Public Functions** | `PascalCase` | `Lexer`, `Parser`, `Resolver`, `GenerateCode` |
| **Function-like Macros** | `snake_case` | `check_mem` (treated like functions for readability) |

---

## Memory Management

If your contribution involves dynamic memory allocation (e.g., adding a new ASM backend, expanding the parser, or managing new data structures), **you must use the `check_mem` utility**. 

This ensures safe reallocation and prevents memory leaks or crashes:
```c
check_mem(array, count, capacity, type, errorMsg);
```
*Do not use raw `realloc` this macro checks the `capacity` and the `count` and realloc the memory by default if needed*

---

## Code Generation Guide

If you are adding support for a new architecture (e.g., ARM64, RISC-V) or modifying how USM translates to Assembly, please refer to the **`CODEGEN.md`** file in the repository. 

It contains:
- The expected output format for different targets.
- How to map USM opcodes to target-specific instructions.
- ABI and stack alignment rules you must respect.

---

Thank you for helping make USM a better educational tool for low-level programming!
```