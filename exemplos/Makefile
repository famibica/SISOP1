#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

all: exemplo mandel teste_vetor barbeiro filosofos prodcons series

exemplo: exemplo.c $(LIB_DIR)/libthread.a
	$(CC) -o exemplo exemplo.c -L$(LIB_DIR) -lthread -Wall

teste_vetor: teste_vetor.c $(LIB_DIR)/libthread.a
	$(CC) -o teste_vetor teste_vetor.c -L$(LIB_DIR) -lthread -Wall

mandel: mandel.c $(LIB_DIR)/libthread.a
	$(CC) -o mandel mandel.c -L$(LIB_DIR) -lthread -lm -Wall

barbeiro: barbeiro.c $(LIB_DIR)/libthread.a
	$(CC) -o barbeiro barbeiro.c -L$(LIB_DIR) -lthread -lm -Wall

filosofos: filosofos.c $(LIB_DIR)/libthread.a
	$(CC) -o filosofos filosofos.c -L$(LIB_DIR) -lthread -lm -Wall

prodcons: prodcons.c $(LIB_DIR)/libthread.a
	$(CC) -o prodcons prodcons.c -L$(LIB_DIR) -lthread -lm -Wall

series: series.c $(LIB_DIR)/libthread.a
	$(CC) -o series series.c -L$(LIB_DIR) -lthread -lm -Wall

clean:
	rm -rf exemplo mandel teste_vetor barbeiro filosofos prodcons series *.o *~




