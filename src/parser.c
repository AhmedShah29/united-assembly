#include <stdio.h>
#include <stdlib.h>
#include "include/parser.h"

Instruction* parse(const token *tokens, size_t tokenCount, size_t *outInstructionCount);