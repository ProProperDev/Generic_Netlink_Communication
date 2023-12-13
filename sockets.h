#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>

#ifndef INCORRECT_VALUE
#define INCORRECT_VALUE -1
#endif

#ifndef CODE_OF_SUCCESS
#define CODE_OF_SUCCESS 0
#endif

int create_socket(int domain, int type, int protocol);

int bind_socket(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

socklen_t fill_sockaddr_nl(struct sockaddr_nl* addr_nl_struct,
                            short family, pid_t  process_pid, unsigned long groups);