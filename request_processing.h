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
#include "client_init_free.h"
#include "calc_functions.h"
#include "sockets.h"
#include "cmd_calculate.h"

#ifndef INCORRECT_VALUE
#define INCORRECT_VALUE -1
#endif

#define MAX_MESSAGE_SIZE 512

typedef enum {
    CMD_CALCULATE = 1,
    CMD_UNKNOWN
} message_command;

void server_request_processing();

void client_request_processing(FILE* stream);

message_command receive_gnl_message(char** received_message, int listen_socket);

int send_gnl_message(const char* message, int socket_for_send);

char* read_jsonstr_from_input(FILE* stream);
