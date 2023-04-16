client: source/client.c
	gcc -o client source/client.c -g

serv: source/server.c
	gcc -o server/serv source/server.c -g

server_gui: source/server_gui.c
	gcc -o server/server_gui source/server_gui.c -g -lncurses

make_all: source/server.c source/client.c source/server_gui.c
	gcc -o server/server_gui source/server_gui.c -g -lncurses
	gcc -o server/serv source/server.c -g
	gcc -o client source/client.c -g

run_tests:
	cd testing ; ./checksum_small_test.sh

remove_all:
	rm server/serv server/server_gui client

clean: 
	rm *.out ; rm server/*.out
