#Team members: Aaron Chasse, Jake Borg, Doug Liu
#CSE 220 - Lab 3


#!/bin/bash
# File: Makefile

lister: main.o print.o scanner.o
	gcc main.o print.o scanner.o -o lister

main.o: main.c
	gcc -c main.c
print.o: print.c
	gcc -c print.c
scanner.o: scanner.c
	gcc -c scanner.c



