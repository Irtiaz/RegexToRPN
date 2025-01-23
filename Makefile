command=gcc -Wall -Wextra -Werror -ansi -pedantic -fsanitize=address,null,undefined,leak $(shell find . -name "*.c")
all:
	$(command)
dev:
	bear -- $(command) -g3
