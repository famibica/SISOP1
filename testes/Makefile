#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

all: testFilosofosCorrigido

testFilosofosCorrigido: testFilosofosCorrigido.c $(LIB_DIR)/libcthread.a
	$(CC) -o testFilosofosCorrigido testFilosofosCorrigido.c -L$(LIB_DIR) -lcthread -Wall

clean:
	rm -rf testFilosofosCorrigido *.o *~




