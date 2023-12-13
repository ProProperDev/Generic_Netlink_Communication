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

#include "client_init_free.h"
#include "request_processing.h"

int main() {
    gennl_client_init();
    printf("Initialization of the client for communication " 
            "via generic netlink protocol has been completed succesfully!\n");
    client_request_processing(stdin);
    gennl_client_free();
    printf("Generic Netlink client has been free all resources!\n");
    return 0;
}
