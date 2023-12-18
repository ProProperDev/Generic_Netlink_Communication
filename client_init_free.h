#pragma once

#include <errno.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>

#include "sockets.h"
#include "request_processing.h"

#ifndef CODE_OF_SUCCESS
#define CODE_OF_SUCCESS 0
#endif

#ifndef SERV_INFOFILE_PATH
#define SERV_INFOFILE_PATH "/tmp/gennl_server_info"
#endif

#ifndef BUFSIZE
#define BUFSIZE 255
#endif

extern struct sockaddr_nl sender_addr;

static struct sockaddr_nl client_addr = {0};
int client_gennl_socket;

void gennl_client_init();

int create_and_bind_client_socket();

int read_server_infofile();

void gennl_client_free();
