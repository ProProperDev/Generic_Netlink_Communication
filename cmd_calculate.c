#include "cmd_calculate.h"

// Обработка команды запроса CMD_CALCULATE
int cmd_calc_processing(const char* request) {
    char* action = NULL;

    // Типы аргументов и результата заранее неизвестны, поэтому
    // используем typed_variable
    typed_variable arg_1 = {0}, arg_2 = {0}, result = {0};

    char* response = NULL, *error_response = NULL;

    // Парсим JSON запрос
    if (parse_calc_params(request, &action, &arg_1, &arg_2) != CODE_OF_SUCCESS) {
        safe_free(action);
        safe_free(arg_1.data);
        safe_free(arg_2.data);

        error_response = compose_cmd_calc_error_answer("parsing params error");

        send_gnl_message(error_response, gennl_socket);

        safe_free(error_response);

        return INCORRECT_VALUE;
    }

    // Производим расчёт. Return true: расчёт прошёл успешно
    if (!is_success_calculate(&result, action, &arg_1, &arg_2)) {
        safe_free(action);
        safe_free(arg_1.data);
        safe_free(arg_2.data);
        safe_free(result.data);

        error_response = compose_cmd_calc_error_answer("calculating error");

        send_gnl_message(error_response, gennl_socket);

        safe_free(error_response);

        return INCORRECT_VALUE;
    }

    response = compose_cmd_calc_answer(&result);

///////////////////////////Для отладки////////////////////////////////////
    if (response != NULL) {
        printf("Response: %s\n", response);
    }
    if (error_response != NULL) {
        printf("Response: %s\n", error_response);
    }
///////////////////////////////////////////////////////////////////////////

    if (send_gnl_message(response, gennl_socket) < 0) {
        safe_free(action);
        safe_free(arg_1.data);
        safe_free(arg_2.data);
        safe_free(result.data);
        safe_free(response);
        safe_free(error_response);

        return INCORRECT_VALUE;
    }

    return CODE_OF_SUCCESS;
}

int parse_calc_params(const char* json_request, char** action,
                        typed_variable* arg1, typed_variable* arg2) {
 
    struct json_object* raw_json_obj = json_tokener_parse(json_request);
    if (raw_json_obj == NULL) {
        printf("Error parsing JSON\n");
        return INCORRECT_VALUE;
    }

    struct json_object *action_obj, *arg_1_obj, *arg_2_obj;
 
    // Извлекаем значение поля "action"
    if (json_object_object_get_ex(raw_json_obj, "action", &action_obj)) {
        if (json_object_is_type(action_obj, json_type_string)) {        
            *action = strdup(json_object_get_string(action_obj));
        } else {
            printf("Invalid value \"action\"\n");
            json_object_put(raw_json_obj);
            return INCORRECT_VALUE;
        }
    } else {
        printf("Error parsing \"action\" field\n");
        json_object_put(raw_json_obj);
        return INCORRECT_VALUE;
    } 


    // Извлекаем значение поля "argument_1"
    if (json_object_object_get_ex(raw_json_obj, "argument_1", &arg_1_obj)) {
        if (json_object_is_type(arg_1_obj, json_type_int)) {
            arg1->data = (int*)malloc(sizeof(int));
            arg1->type = TYPE_INT;
            if (arg1->data == NULL) {
                perror("Can't allocate memory for <int> arg1");
                json_object_put(raw_json_obj);
                return INCORRECT_VALUE;
            }
            memset(arg1->data, 0, sizeof(int));
            int parsed_arg1 = json_object_get_int(arg_1_obj);
            memcpy(arg1->data, &parsed_arg1, sizeof(int));
        } else if (json_object_is_type(arg_1_obj, json_type_double)) {
            arg1->data = (double*)malloc(sizeof(double));
            arg1->type = TYPE_DOUBLE;
            if (arg1->data == NULL) {
                perror("Can't allocate memory for <double> arg1");
                json_object_put(raw_json_obj);
                return INCORRECT_VALUE;
            }
            memset(arg1->data, 0, sizeof(double));
            double parsed_arg1 = json_object_get_double(arg_1_obj);
            memcpy(arg1->data, &parsed_arg1, sizeof(double));
        } else {
            printf("Invalid value \"argument_1\"\n");
            json_object_put(raw_json_obj);
            return INCORRECT_VALUE;
        }
    } else {
        printf("Error parsing \"argument_1\" field\n");
        json_object_put(raw_json_obj);
        return INCORRECT_VALUE;
    }

    // Извлекаем значение поля "argument_2"
    if (json_object_object_get_ex(raw_json_obj, "argument_2", &arg_2_obj)) {
        if (json_object_is_type(arg_2_obj, json_type_int)) {
            arg2->data = (int*)malloc(sizeof(int));
            arg2->type = TYPE_INT;
            if (arg2->data == NULL) {
                perror("Can't allocate memory for <int> arg2");
                json_object_put(raw_json_obj);
                return INCORRECT_VALUE;
            }
            memset(arg2->data, 0, sizeof(int));
            int parsed_arg2 = json_object_get_int(arg_2_obj);
            memcpy(arg2->data, &parsed_arg2, sizeof(int));
        } else if (json_object_is_type(arg_2_obj, json_type_double)) {
            arg2->data = (double*)malloc(sizeof(double));
            arg2->type = TYPE_DOUBLE;
            if (arg2->data == NULL) {
                perror("Can't allocate memory for <double> arg2");
                json_object_put(raw_json_obj);
                return INCORRECT_VALUE;
            }
            memset(arg2->data, 0, sizeof(double));
            double parsed_arg2 = json_object_get_double(arg_2_obj);
            memcpy(arg2->data, &parsed_arg2, sizeof(double));
        } else {
            printf("Invalid value \"argument_2\"\n");
            json_object_put(raw_json_obj);
            return INCORRECT_VALUE;
        }
    } else {
        printf("Error parsing \"argument_2\" field\n");
        json_object_put(raw_json_obj);
        return INCORRECT_VALUE;
    }

    json_object_put(raw_json_obj);

    return CODE_OF_SUCCESS;
}

int resolve_action_type(const char* action) {
    if (strcasestr("+", action) || strcasestr("plus", action)) {
        return ADD;
    }
    if (strcasestr("-", action) || strcasestr("minus", action)) {
        return SUBTRACT;
    }
    if (strcasestr("*", action) || strcasestr("multiply", action)) {
        return MULTIPLY;
    }
    if (strcasestr("/", action) || strcasestr("divide", action)) {
        return DIVIDE;
    }

    return UNKNOWN_ACTION;
}

bool is_success_calculate(typed_variable* result, const char* action,
                            const typed_variable* arg_1, const typed_variable* arg_2) {
    const enum action_type action_type = resolve_action_type(action);

    switch(action_type) {
        case ADD:
            add(result, arg_1, arg_2);
            break;

        case SUBTRACT:
            subtract(result, arg_1, arg_2);
            break;

        case MULTIPLY:
            multiply(result, arg_1, arg_2);
            break;

        case DIVIDE:
            divide(result, arg_1, arg_2);
            break;
    }

    if (result->data == NULL) {
        return false;
    }

    return true;
}

// Обязательно использование возвращаемого значения, т.к. функция возвращает
// указатель на выделенную память с помощью malloc()
__attribute__((warn_unused_result)) char* compose_cmd_calc_error_answer(char* error_reason) {
     // Создание JSON-ответа
    json_object *json_obj_answer;
    json_obj_answer = json_object_new_object();

    json_object_object_add(json_obj_answer, "error", json_object_new_string(error_reason));

    int answer_string_size = strlen(json_object_to_json_string(json_obj_answer)) + 1;
    char* response = malloc(answer_string_size);
    if (response == NULL) {
        perror("Can't allocate memory for response with malloc()");
        return NULL;
    }
    memset(response, '\0', answer_string_size);

    strcpy(response, json_object_to_json_string(json_obj_answer));

    return response;
}

// Обязательно использование возвращаемого значения, т.к. функция возвращает
// указатель на выделенную память с помощью malloc()
__attribute__((warn_unused_result)) char* compose_cmd_calc_answer(const typed_variable* result) {
    // Создание JSON-ответа
    json_object *json_obj_answer;
    json_obj_answer = json_object_new_object();

    if (result->type == TYPE_INT) {
        int adapted_result = *((int*)result->data);
        json_object_object_add(json_obj_answer, "result", json_object_new_int(adapted_result));
    } else if (result->type == TYPE_DOUBLE) {
        double adapted_result = *((double*)result->data);
        json_object_object_add(json_obj_answer, "result", json_object_new_double(adapted_result));
    }
    int answer_string_size = strlen(json_object_to_json_string(json_obj_answer)) + 1;
    char* response = malloc(answer_string_size);
    if (response == NULL) {
        perror("Can't allocate memory for response with malloc()");
        return NULL;
    }
    memset(response, '\0', answer_string_size);

    strcpy(response, json_object_to_json_string(json_obj_answer));

    return response;
}

void safe_free(void* ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}