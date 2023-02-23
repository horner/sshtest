
all: ssh_host_rsa_key server

server: main.c
	cc -o server main.c -lssh

ssh_host_rsa_key:
	ssh-keygen -f ssh_host_rsa_key -N '' -t rsa