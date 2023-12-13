#include "request_processing.h"

struct sockaddr_nl sender_addr = {0};

void server_request_processing() {
    while (true) {
        char* received_message;

        message_command message_cmd = receive_gnl_message(&received_message, gennl_socket);
        if (message_cmd == CMD_CALCULATE) {
            if (cmd_calc_processing(received_message) != CODE_OF_SUCCESS) {
                printf("Error CMD_CALCULATE processing\n");
            }
        }
        safe_free(received_message);
    }
}

char* read_jsonstr_from_input(FILE* stream) {
    char* input_str = NULL;
    size_t len = 0;

    ssize_t read_bytes = getline(&input_str, &len, stream);

    if (read_bytes < 0) {
        printf("Error reading input\n");
        return NULL;
    }

    // Убедимся, что строка завершается нулевым символом
    if (input_str[read_bytes - 1] == '\n') {
        input_str[read_bytes - 1] = '\0';
    }

    return input_str;
}

void client_request_processing(FILE* stream) {
    while(true) {
        char* received_message = NULL;

        printf("Enter a json request:\n");
        char* jsonstr_request = read_jsonstr_from_input(stream);
        if (jsonstr_request == NULL) {
            printf("Invalid input\n");
            continue;
        }
        if (strcasestr("exit", jsonstr_request)) {
            safe_free(jsonstr_request);
            break;
        }

        ssize_t sent_bytes = send_gnl_message(jsonstr_request, client_gennl_socket);
        if (sent_bytes < 0) {
            safe_free(jsonstr_request);
            printf("Error sending. Try again.\n");
            continue;
        }

        receive_gnl_message(&received_message, client_gennl_socket);
        
        safe_free(jsonstr_request);
        safe_free(received_message);
    }
    printf("GOODBYE!\n");
}

int send_gnl_message(const char* message, int socket_for_send) {
    struct nlmsghdr *nlh_;
    struct genlmsghdr *genlhdr_;
    struct iovec iov_ = {0};
    struct msghdr msg_ = {0};

    // Выделяем память для Netlink-сообщения
    nlh_ = (struct nlmsghdr *)malloc(NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE));
    if (nlh_ == NULL) {
        perror("Can't allocate memory for netlink message with malloc()");
        return INCORRECT_VALUE;
    }

    // Заполняем Netlink-сообщение
    nlh_->nlmsg_len = NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE);
    nlh_->nlmsg_pid = getpid();  // PID отправителя
    nlh_->nlmsg_flags = 0;

    // Заполняем заголовок Generic Netlink
    genlhdr_ = (struct genlmsghdr *)NLMSG_DATA(nlh_);
    genlhdr_->cmd = CMD_CALCULATE;

    // Копируем данные в Netlink-сообщение
    strcpy((char *)genlhdr_ + GENL_HDRLEN, message);

    // Заполняем структуру iovec
    iov_.iov_base = (void *)nlh_;
    iov_.iov_len = nlh_->nlmsg_len;

    // Заполняем структуру msghdr
    memset(&msg_, 0, sizeof(msg_));
    msg_.msg_name = (void *)&sender_addr;
    msg_.msg_namelen = sizeof(sender_addr);
    msg_.msg_iov = &iov_;
    msg_.msg_iovlen = 1;

    // Отправляем Netlink-сообщение
    ssize_t sent_bytes = sendmsg(socket_for_send, &msg_, 0);
    if (sent_bytes < 0) {
        perror("Error generic netlink message sending");
        safe_free(nlh_);
        return INCORRECT_VALUE;
    }
    printf("Sent bytes:%ld\n", sent_bytes);
    safe_free(nlh_);

    return sent_bytes;
}

message_command receive_gnl_message(char** received_message, int listen_socket) {
    struct nlmsghdr *nlh;
    struct genlmsghdr *genlhdr;
    struct iovec iov = {0};
    struct msghdr msg = {0};

    // Выделяем память для Netlink-сообщения
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE));
    if (nlh == NULL) {
        perror("Can't allocate memory for netlink message with malloc()");
        return INCORRECT_VALUE;
    }

    // Заполняем структуру iovec
    iov.iov_base = (void*)nlh;
    iov.iov_len = NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE);

    // Заполняем структуру msghdr        
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // Получаем Netlink-сообщение
    ssize_t bytes_received = recvmsg(listen_socket, &msg, 0);
    if (bytes_received < 0) {
        perror("Message receiving Error");
        safe_free(nlh);
        return INCORRECT_VALUE;
    }

    printf("\nSender PID: %d\n", nlh->nlmsg_pid); // Для отладки

    // Проверяем, что получено Generic Netlink-сообщение
    if (nlh->nlmsg_type == NLMSG_DONE) {
        // Получено пустое сообщение NLMSG_DONE, игнорируем
        printf("Error: empty gnl message received\n");
        safe_free(nlh);
        return INCORRECT_VALUE;
    }

    // Заполнение структуры sockaddr_nl адреса отправителя
    // nlh->nlmsg_pid - PID процесса отправителя
    fill_sockaddr_nl(&sender_addr, AF_NETLINK, nlh->nlmsg_pid, 0);
       
    // Получено Generic Netlink-сообщение
    genlhdr = (struct genlmsghdr *)NLMSG_DATA(nlh);
    char* buf_for_message = (char*)genlhdr + GENL_HDRLEN;
        
    *received_message = (char*)malloc(strlen(buf_for_message) + 1);
    if (*received_message == NULL) {
        perror("Can't allocate memory for received message with malloc()");
        return INCORRECT_VALUE;
    }

    strcpy(*received_message, buf_for_message);
    printf("Received message: %s\n", *received_message);
    message_command command = genlhdr->cmd;

    safe_free(nlh);

    return command;
}
