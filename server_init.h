#pragma once

#include <errno.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>

#include "sockets.h"

#ifndef CODE_OF_SUCCESS
#define CODE_OF_SUCCESS 0
#endif

#define SERV_INFOFILE_PATH "/tmp/gennl_server_info"
// На данном этапе хранить в файле будем только PID процесса сервера
#define SERV_INFOFILE_SIZE (sizeof(pid_t))

static struct sockaddr_nl server_addr = {0};
int gennl_socket;

void gennl_server_init();

int create_and_bind_server_socket();

int fill_server_infofile();

void gennl_server_free();