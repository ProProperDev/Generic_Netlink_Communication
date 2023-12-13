#include "sockets.h"

int create_socket(int domain, int type, int protocol) {
    int result = socket(domain, type, protocol);
    if (result == INCORRECT_VALUE) {
        perror("Socket creation failed");
        return INCORRECT_VALUE;
    }
    return result; // Возвращаем дескриптор
}

int bind_socket(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int result = bind(sockfd, addr, addrlen);
    if (result == INCORRECT_VALUE) {
        perror("Can't associates a socket with an address. Bind failed");
        return INCORRECT_VALUE;
    }
    return CODE_OF_SUCCESS;
}

socklen_t fill_sockaddr_nl(struct sockaddr_nl* addr_nl_struct,
                            short family, pid_t  process_pid, unsigned long groups) {

    addr_nl_struct->nl_family = family;
    addr_nl_struct->nl_pid = process_pid;
    addr_nl_struct->nl_groups = groups;

    return sizeof(struct sockaddr_nl); 
} 