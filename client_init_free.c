#include "client_init_free.h"

void gennl_client_init() {
    if (create_and_bind_client_socket() != CODE_OF_SUCCESS) {
        printf("Client initialization FAILED!\n");
        exit(EXIT_FAILURE);
    }

    if (read_server_infofile() != CODE_OF_SUCCESS) {
        printf("Client initialization FAILED!\n");
        gennl_client_free();
        exit(EXIT_FAILURE);
    }  
}

int create_and_bind_client_socket() {
    client_gennl_socket = create_socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (client_gennl_socket <= 0) {
        return errno;
    }

    // Заполнение структуры sockaddr_nl для клиента
    socklen_t client_addr_len = fill_sockaddr_nl(&client_addr, AF_NETLINK, getpid(), 0);

    if (bind_socket(client_gennl_socket, (struct sockaddr*)&client_addr, client_addr_len) != CODE_OF_SUCCESS) {
        close(client_gennl_socket);
        return errno;
    }

    return CODE_OF_SUCCESS;
}

int read_server_infofile() {
    int fd, len;
    char buf[BUFSIZE] = {0};
    pid_t server_pid;
    FILE * fstream_ = fopen(SERV_INFOFILE_PATH, "r");
    if (fstream_ == NULL) {
        perror("Can't open server infofile for reading params");
        return errno;
    }
    if (fscanf(fstream_,"%d", &server_pid) <= 0) {
        perror("Can't read server infofile for set params");
        fclose(fstream_);
        return errno;
    }

    fill_sockaddr_nl(&sender_addr, AF_NETLINK, server_pid, 0);
    fclose(fstream_);

    return CODE_OF_SUCCESS;
}

void gennl_client_free() {
    close(client_gennl_socket);
}
