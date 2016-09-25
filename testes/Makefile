#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

all: testFilosofosCorrigido testCreate testYield testJoin testSemaforo testIdentify

testFilosofosCorrigido: testFilosofosCorrigido.c $(LIB_DIR)/libcthread.a
	$(CC) -o testFilosofosCorrigido testFilosofosCorrigido.c -L$(LIB_DIR) -lcthread -lm -Wall

testCreate: testCreate.c $(LIB_DIR)/libcthread.a
	$(CC) -o testCreate testCreate.c -L$(LIB_DIR) -lcthread -lm -Wall

testYield: testYield.c $(LIB_DIR)/libcthread.a
	$(CC) -o testYield testYield.c -L$(LIB_DIR) -lcthread -lm -Wall

testJoin: testJoin.c $(LIB_DIR)/libcthread.a
	$(CC) -o testJoin testJoin.c -L$(LIB_DIR) -lcthread -lm -Wall

testSemaforo: testSemaforo.c $(LIB_DIR)/libcthread.a
	$(CC) -o testSemaforo testSemaforo.c -L$(LIB_DIR) -lcthread -lm -Wall

testIdentify: testIdentify.c $(LIB_DIR)/libcthread.a
	$(CC) -o testIdentify testIdentify.c -L$(LIB_DIR) -lcthread -lm -Wall

clean:
	rm -rf testFilosofosCorrigido testIdentify testSemaforo testJoin testYield testCreate *.o *~




