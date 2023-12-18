COMMON_SOURCES = calc_functions.c cmd_calculate.c request_processing.c sockets.c client_init_free.c server_init.c -ljson-c

all: 
	gcc $(COMMON_SOURCES) client_main.c -o client
	gcc $(COMMON_SOURCES) server_main.c -o server

clean:
	rm -f client server
