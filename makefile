.PHONY: run debug test compile

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -Iinclude
# -ftrivial-auto-var-init=zero
SRC = src

run: CFLAGS += -O3
run: compile
	./bin/exe

debug: CFLAGS += -g -fdiagnostics-color=always
debug: compile

test: SRC = test
test: compile
	./bin/exe

compile:
	$(CC) $(CFLAGS) $(SRC)/*.c -o bin/exe

