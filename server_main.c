#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>

#include "sockets.h"
#include "server_init.h"
#include "request_processing.h"

#ifndef CODE_OF_SUCCESS
#define CODE_OF_SUCCESS 0
#endif

int main() {
    gennl_server_init();
    printf("Initialization of the server for communication " 
            "via generic netlink protocol has been completed succesfully!\n");
    server_request_processing();
    gennl_server_free();
    printf("Generic Netlink server has been free all resources!\n");
    return 0;
}
