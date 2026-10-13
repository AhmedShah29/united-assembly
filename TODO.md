# USM Compiler TODO List

**Phase 1: Advanced Error Reporting System**
- [x] Design Error Enums & Categories (Syntax, Reference, System).
- [ ] Implement `Error` and `ErrorList` structs to collect errors instead of exiting immediately.
- [ ] Implement `push_error` function to append errors to the array.
- [ ] Implement Source Snippet Extraction (Pointer arithmetic to find line start/end).
- [ ] Implement the Pointer (`^^^`) logic based on token length.
- [ ] Add ANSI Color Codes (Bold dark red for "Error:", cyan for file paths).
- [ ] Update Lexer, Parser, and Resolver to use `ErrorList` and check `hasErrors`.

**Phase 2: Lexer Optimization (Numbers & Union)**
- [ ] Update `Token` Struct in `lexer.h`: Replace `strId` with a `union` containing `uint32_t strId` and `int64_t imm`.
- [ ] Optimize `lexer.c`: Parse `TOKEN_INT` immediately using `strtoll` and skip `pool_intern` for numbers.
- [ ] Implement Perfect Hashmap for Syntax: Replace `get_token_enum()` if/else chain with a static lookup array.

**Phase 3: Core Refactoring (Parser & String Pool Integration)**
- [ ] Update `parser.h` Structs: Change `char name[64]` and `char data[64]` in the `Operand` union to a single `uint32_t id`.
- [ ] Refactor `parser.c`: Pass `Pool* pool` to the `Parser` function and use `pool_get()`.
- [ ] Update Parser to use the new Lexer `Token` union (e.g., read `tokens[i].value.imm` directly instead of calling `parse_immediate`).
- [ ] Refactor Parser DRY: Create helper functions (`expect_token`, `consume_comma`, `parse_operand`).

**Phase 4: Semantic Analysis & Resolver Improvements**
- [ ] Implement Type Tracking: Store variable types/sizes (e.g., String, Int, Byte) in the Resolver's HashMap during the first pass.
- [ ] Add checks for illegal operations: Prevent arithmetic (ADD, SUB, DIV, etc.) on string variables.
- [ ] Add checks for register/size mismatches: Warn or error if moving a 64-bit value into an 8-bit register without explicit casting.

**Phase 5: CLI & Debugging**
- [ ] Implement `-d` / `--debug` Flag: Parse `argv` in `main.c` to detect the debug flag.
- [ ] Wrap Lexer/Parser debug prints inside `if (isDebug)` blocks.

**Phase 6: Feature Expansion (Floating-Point)**
- [ ] Add `F32` and `F64` sizes and registers (`RF0` - `RF7`).
- [ ] Implement Float instructions (`FMOV`, `FADD`, `FSUB`, ...).

**Phase 7: CodeGen Refactoring**
- [ ] Refactor `x86_64.c` to use the new AST and String Pool IDs (`pool_get(pool, instr.val.id)`).
- [ ] Update CodeGen to handle floating-point instructions and optimized integer tokens.

**Phase 8: Add a new backend**
- [ ] ???