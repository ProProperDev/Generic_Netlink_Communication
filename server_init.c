#include "server_init.h"

int gennl_socket;

void gennl_server_init() {
    if (create_and_bind_server_socket() != CODE_OF_SUCCESS) {
        printf("Server initialization FAILED!\n");
        exit(EXIT_FAILURE);
    }

    if (fill_server_infofile() != CODE_OF_SUCCESS) {
        printf("Server initialization FAILED!\n");
        gennl_server_free();
        exit(EXIT_FAILURE);
    }  
}

int create_and_bind_server_socket() {
    gennl_socket = create_socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (gennl_socket <= 0) {
        return errno;
    }

    // Заполнение структуры sockaddr_nl для сервера
    socklen_t server_addr_len = fill_sockaddr_nl(&server_addr, AF_NETLINK, getpid(), 0);

    if (bind_socket(gennl_socket, (struct sockaddr*)&server_addr, server_addr_len) != CODE_OF_SUCCESS) {
        close(gennl_socket);
        return errno;
    }

    return CODE_OF_SUCCESS;
}

int fill_server_infofile() {
    FILE* infofile_ptr = fopen(SERV_INFOFILE_PATH, "w+");
    if (infofile_ptr == NULL) {
        perror("Can't open server infofile for filling");
        return errno;
    }

    if (truncate(SERV_INFOFILE_PATH, SERV_INFOFILE_SIZE) != CODE_OF_SUCCESS) {
        perror("Can't truncate server infofile");
        fclose(infofile_ptr);
        return errno;
    }
    
    int recorded_size = fprintf(infofile_ptr,"%d", getpid());
    if (recorded_size < 0) {
        perror("Can't record data in server infofile");
        fclose(infofile_ptr);
        return errno;
    }

    fclose(infofile_ptr);

    return CODE_OF_SUCCESS;
}

void gennl_server_free() {
    close(gennl_socket);
    remove(SERV_INFOFILE_PATH);
}
