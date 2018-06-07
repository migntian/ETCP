.PHONY:clean
all: server client 

server:server.c -lpthread
	gcc -o $@ $^
client:client.c
	gcc -o $@ $^
clean:
	rm server client
