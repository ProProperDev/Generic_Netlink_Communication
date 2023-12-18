#pragma once

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <json-c/json.h>

#include "server_init.h"
#include "calc_functions.h"
#include "request_processing.h"

#ifndef INCORRECT_VALUE
#define INCORRECT_VALUE -1
#endif

#ifndef MAX_MESSAGE_SIZE
#define MAX_MESSAGE_SIZE 512
#endif

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

/*Supported actions for CMD_CALCULATE*/
enum action_type {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    UNKNOWN_ACTION
};

/****************************CMD_CALCULATE functions****************************/

int cmd_calc_processing(const char* request);

int parse_calc_params(const char* json_request, char** action,
                        typed_variable* arg1, typed_variable* arg2);

int resolve_action_type(const char* action);

bool is_success_calculate(typed_variable* result, const char* action,
                            const typed_variable* arg_1, const typed_variable* arg_2);

// Обязательно использование возвращаемого значения, т.к. функция возвращает
// указатель на выделенную память с помощью malloc()
__attribute__((warn_unused_result)) char* compose_cmd_calc_error_answer(char* error_reason);

// Обязательно использование возвращаемого значения, т.к. функция возвращает
// указатель на выделенную память с помощью malloc()
__attribute__((warn_unused_result)) char* compose_cmd_calc_answer(const typed_variable* result);

void safe_free(void* ptr);

/*******************************************************************************/
