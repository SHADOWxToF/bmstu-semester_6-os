
app : client.out server.out

server.out : server.c
	gcc server.c -o server.out

client.out : client.c
	gcc client.c -o client.out

clean : 
	rm *.out *.soc

PHONY : app clean
