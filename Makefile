all:
	gcc calc_functions.c cmd_calculate.c -ljson-c request_processing.c sockets.c client_init_free.c client_main.c -o client
	gcc server_main.c sockets.c server_init.c request_processing.c calc_functions.c cmd_calculate.c -ljson-c -o server

clean:
	rm client server
