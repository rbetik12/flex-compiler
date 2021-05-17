#pragma once

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
    void insert(char* var_name, int lineno);
    void dump_table(FILE* file);
#ifdef __cplusplus
}
#endif