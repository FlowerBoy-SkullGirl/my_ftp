HEADERS := source/$(wildcard *.h)
BINARIES := server/serv server/server_gui client testing/hash_driver
TARGETS := client serv server_gui drivers

all: $(TARGETS)

client: source/client.c $(HEADERS)
	gcc -o client source/client.c -g

serv: source/server.c $(HEADERS)
	gcc -o server/serv source/server.c -g

server_gui: source/server_gui.c $(HEADERS)
	gcc -o server/server_gui source/server_gui.c -g -lncurses


drivers: source/hash_driver.c $(HEADERS)
	gcc -o testing/hash_driver source/hash_driver.c -g

run_tests:
	cd testing ; ./checksum_small_test.sh
	cd testing ; ./hash_driver

clean: 
	rm $(BINARIES)
