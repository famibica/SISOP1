#!/bin/bash

gcc -o mandel mandel.c -L../lib -lthread -Wall; gcc -o poema poema.c -L../lib -lthread -Wall; gcc -o simples simples.c -L../lib -lthread -Wall; gcc -o teste1 teste1.c -L../lib -lthread -Wall; gcc -o teste2 teste2.c -L../lib -lthread -Wall; gcc -o teste3 teste3.c -L../lib -lthread -Wall; gcc -o vetor vetor.c -L../lib -lthread -Wall;
