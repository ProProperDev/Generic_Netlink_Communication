#pragma once

#include "cmd_calculate.h"

#ifndef TYPED_VARIABLE
#define TYPED_VARIABLE

// Перечисление для представления типов данных
typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_UNKNOWN
} variable_type;

// Структура для хранения указателя на данные и их типа
typedef struct {
    void* data;
    variable_type type;
} typed_variable;

#endif

void add(typed_variable* result, const typed_variable* arg_1,
            const typed_variable* arg_2);

void subtract(typed_variable* result, const typed_variable* arg_1,
                const typed_variable* arg_2);

void multiply(typed_variable* result, const typed_variable* arg_1,
                const typed_variable* arg_2);

void divide(typed_variable* result, const typed_variable* arg_1,
                const typed_variable* arg_2);
