
pre_build: cake.c
	$(CC) -Wall -Wextra -o cake cake.c

post_build: build_linux
	./cake-linux

build_linux: pre_build
	./cake

build: post_build 

