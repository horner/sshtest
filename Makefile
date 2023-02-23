
all: ssh_host_rsa_key server-c server-cpp host_key.rsa host_key.ecdsa

server-c: main.c
	cc -o server-c main.c -lssh

server-cpp: main.cpp
	c++ -std=gnu++17 -o server-cpp main.cpp -lssh

ssh_host_rsa_key:
	ssh-keygen -f ssh_host_rsa_key -N '' -t rsa


host_key.%:
	ssh-keygen -q -N '' -C '' -t $(@:host_key.%=%) -f $@
