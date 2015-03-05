# makefile
CC = gcc
CFLAGS = 
DEPS = vbi.h
OBJ = txt2vbi.o vbi.o 

all: txt2vbi

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

txt2vbi: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
