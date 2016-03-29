# default config
tag = -i
CC = g++ -std=c++11 -O2 -Wno-deprecated -w

# find current directory and use /src and /bin
# include /data and /logs if necessary
CURR_DIR = $(shell pwd)

bbst: main.o 
	$(CC) -o bbst  main.o
	
main.o:  main.cc red_black.h
	$(CC)  -g -c main.cc
