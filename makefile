client: source/client.c
	gcc -o client source/client.c -g
serv: source/server.c
	gcc -o server/serv source/server.c -g
server_gui: source/server_gui.c
	gcc -o server/server_gui source/server_gui.c -g -lncurses
clean: 
	rm *.out ; rm server/*.out
