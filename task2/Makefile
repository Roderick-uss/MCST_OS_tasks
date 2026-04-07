CFLAGS += -Wextra -Wall

B_PREFIX = bin/
O_PREFIX = objects/
S_PREFIX = sources/
H_PREFIX = headers/
T_PREFIX = text_file/

F_PREFIX = fifo/

SOURCES = main chat

OBJECTS := $(patsubst %,$(O_PREFIX)%.o,$(SOURCES))

HEADER_LIST = $(H_PREFIX)*.h

all: main

run: main
	@./$(B_PREFIX)main $(F_PREFIX)$(fifo).fifo

fifo:
	@mkdir -p $(F_PREFIX)
	@mkfifo $(F_PREFIX)$(fifo).fifo

main: $(OBJECTS)
	@mkdir -p $(B_PREFIX)
	@gcc $(CFLAGS) $^ -o $(B_PREFIX)$@
	@gcc $(CFLAGS) $^ -o task2

$(O_PREFIX)%.o: $(S_PREFIX)%.c $(HEADER_LIST)
	@mkdir -p $(O_PREFIX)
	@gcc $(CFLAGS) -I $(H_PREFIX) $< -c -o $@

clean:
	rm -rf $(O_PREFIX)* $(B_PREFIX)* task2 $(F_PREFIX)/
