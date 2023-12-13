#include "calc_functions.h"

void add(typed_variable* result, const typed_variable* arg_1,
            const typed_variable* arg_2) {
    if (arg_1->type == TYPE_DOUBLE || arg_2->type == TYPE_DOUBLE) {
        result->data = malloc(sizeof(double));
        result->type = TYPE_DOUBLE;
        if (result->data == NULL) {
            perror("Can't allocate memory in ADD func");
            return;
        }
        double adapted_arg1 = arg_1->type == TYPE_INT ? (*((int*)arg_1->data)) * 1.0
                                : *((double*)arg_1->data);
        double adapted_arg2 = arg_2->type == TYPE_INT ? (*((int*)arg_2->data)) * 1.0
                                : *((double*)arg_2->data);
        double subresult = adapted_arg1 + adapted_arg2;
                              
        memcpy(result->data, &subresult, sizeof(double));
        return;
    }
    if (arg_1->type == TYPE_INT && arg_2->type == TYPE_INT) {
        result->data = malloc(sizeof(int));
        result->type = TYPE_INT;
        if (result->data == NULL) {
            perror("Can't allocate memory in ADD func");
            return;
        }
        int subresult = *((int*)arg_1->data) + *((int*)arg_2->data);
        memcpy(result->data, &subresult, sizeof(int));
        return;
    }   
}

void subtract(typed_variable* result, const typed_variable* arg_1,
            const typed_variable* arg_2) {
    if (arg_1->type == TYPE_DOUBLE || arg_2->type == TYPE_DOUBLE) {
        result->data = malloc(sizeof(double));
        result->type = TYPE_DOUBLE;
        if (result->data == NULL) {
            perror("Can't allocate memory in SUBTRACT func");
            return;
        }
        double adapted_arg1 = arg_1->type == TYPE_INT ? (*((int*)arg_1->data)) * 1.0
                                : *((double*)arg_1->data);
        double adapted_arg2 = arg_2->type == TYPE_INT ? (*((int*)arg_2->data)) * 1.0
                                : *((double*)arg_2->data);
        double subresult = adapted_arg1 - adapted_arg2;
                              
        memcpy(result->data, &subresult, sizeof(double));
        return;
    }
    if (arg_1->type == TYPE_INT && arg_2->type == TYPE_INT) {
        result->data = malloc(sizeof(int));
        result->type = TYPE_INT;
        if (result->data == NULL) {
            perror("Can't allocate memory in SUBTRACT func");
            return;
        }
        int subresult = *((int*)arg_1->data) - *((int*)arg_2->data);
        memcpy(result->data, &subresult, sizeof(int));
        return;
    } 
}

void multiply(typed_variable* result, const typed_variable* arg_1,
            const typed_variable* arg_2) {
    if (arg_1->type == TYPE_DOUBLE || arg_2->type == TYPE_DOUBLE) {
        result->data = malloc(sizeof(double));
        result->type = TYPE_DOUBLE;
        if (result->data == NULL) {
            perror("Can't allocate memory in MULTIPLY func");
            return;
        }
        double adapted_arg1 = arg_1->type == TYPE_INT ? (*((int*)arg_1->data)) * 1.0
                                : *((double*)arg_1->data);
        double adapted_arg2 = arg_2->type == TYPE_INT ? (*((int*)arg_2->data)) * 1.0
                                : *((double*)arg_2->data);
        double subresult = adapted_arg1 * adapted_arg2;
                              
        memcpy(result->data, &subresult, sizeof(double));
        return;
    }
    if (arg_1->type == TYPE_INT && arg_2->type == TYPE_INT) {
        result->data = malloc(sizeof(int));
        result->type = TYPE_INT;
        if (result->data == NULL) {
            perror("Can't allocate memory in MULTIPLY func");
            return;
        }
        int subresult = *((int*)arg_1->data) * *((int*)arg_2->data);
        memcpy(result->data, &subresult, sizeof(int));
        return;
    } 
}

void divide(typed_variable* result, const typed_variable* arg_1,
            const typed_variable* arg_2) {
    
    if (*((int*)arg_2->data) == (arg_2->type == TYPE_INT ? 0 : 0.0)) {
        printf("Division by zero error in DIVIDE func\n");
        return;
    }
    if (arg_1->type == TYPE_DOUBLE || arg_2->type == TYPE_DOUBLE) {
        result->data = malloc(sizeof(double));
        result->type = TYPE_DOUBLE;
        if (result->data == NULL) {
            perror("Can't allocate memory in DIVIDE func");
            return;
        }
        double adapted_arg1 = arg_1->type == TYPE_INT ? (*((int*)arg_1->data)) * 1.0
                                : *((double*)arg_1->data);
        double adapted_arg2 = arg_2->type == TYPE_INT ? (*((int*)arg_2->data)) * 1.0
                                : *((double*)arg_2->data);
        double subresult = adapted_arg1 / adapted_arg2;
                              
        memcpy(result->data, &subresult, sizeof(double));
        return;
    }
    if (arg_1->type == TYPE_INT && arg_2->type == TYPE_INT) {
        result->data = malloc(sizeof(int));
        result->type = TYPE_INT;
        if (result->data == NULL) {
            perror("Can't allocate memory in DIVIDE func");
            return;
        }
        int subresult = *((int*)arg_1->data) / *((int*)arg_2->data);
        memcpy(result->data, &subresult, sizeof(int));
        return;
    } 
}
