Compilation of server:
	gcc k_s_definitions.c k_s_server.c -o k_s_server
Compilation of client:
	gcc k_s_definitions.c k_s_client.c -o k_s_client

Starting server:
	./k_s_server 15000
Starting client:
	./k_s_client localhost 15000
	
Exit the application:
	the client enters the message: 69