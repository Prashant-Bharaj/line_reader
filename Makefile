CC = gcc
BUFFER_SIZE ?= 32
CFLAGS = -Wall -Wextra -Werror -D BUFFER_SIZE=$(BUFFER_SIZE) -I.

SRC = get_next_line/get_next_line.c get_next_line/get_next_line_utils.c tests/test_get_next_line.c
BIN = tests/test_gnl
EDGE_SRC = get_next_line/get_next_line.c get_next_line/get_next_line_utils.c tests/test_edges.c
EDGE_BIN = tests/test_edges

EMPTY_SRC = get_next_line/get_next_line.c get_next_line/get_next_line_utils.c tests/test_empty.c
EMPTY_BIN = tests/test_empty

INTERLEAVE_SRC = get_next_line/get_next_line.c get_next_line/get_next_line_utils.c tests/test_interleave.c
INTERLEAVE_BIN = tests/test_interleave

STRESS_SRC = get_next_line/get_next_line.c get_next_line/get_next_line_utils.c tests/test_stress.c
STRESS_BIN = tests/test_stress

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

test: $(BIN)
	@echo "Running tests..."
	./$(BIN)

$(EDGE_BIN): $(EDGE_SRC)
	$(CC) $(CFLAGS) $(EDGE_SRC) -o $(EDGE_BIN)

test-edges: $(EDGE_BIN)
	@echo "Running edge tests..."
	./$(EDGE_BIN)

$(EMPTY_BIN): $(EMPTY_SRC)
	$(CC) $(CFLAGS) $(EMPTY_SRC) -o $(EMPTY_BIN)

$(INTERLEAVE_BIN): $(INTERLEAVE_SRC)
	$(CC) $(CFLAGS) $(INTERLEAVE_SRC) -o $(INTERLEAVE_BIN)

$(STRESS_BIN): $(STRESS_SRC)
	$(CC) $(CFLAGS) $(STRESS_SRC) -o $(STRESS_BIN).out

test-more: $(EMPTY_BIN) $(INTERLEAVE_BIN) $(STRESS_BIN)
	@echo "Running more edge tests..."
	./$(EMPTY_BIN)
	./$(INTERLEAVE_BIN)
	./$(STRESS_BIN)

test-all:
	@echo "Running unit test (default BUFFER_SIZE=$(BUFFER_SIZE))";
	$(MAKE) test
	@echo "Running edge tests with BUFFER_SIZE=1";
	$(MAKE) test-edges BUFFER_SIZE=1
	@echo "Running edge tests with BUFFER_SIZE=1024";
	$(MAKE) test-edges BUFFER_SIZE=1024

valgrind: $(BIN)
	@echo "Running under valgrind (requires valgrind installed)"
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN)

valgrind-all:
	@echo "Running valgrind for BUFFER_SIZE=1,32,1024 on all test binaries"
	@for bs in 1 32 1024; do \
		echo "=== BUFFER_SIZE=$$bs: unit test ==="; \
		$(MAKE) -s clean; \
		$(MAKE) -s test BUFFER_SIZE=$$bs; \
		echo "Running valgrind on tests/test_gnl"; \
		valgrind --leak-check=full --show-leak-kinds=all ./tests/test_gnl; \
		echo "=== BUFFER_SIZE=$$bs: edge tests ==="; \
		$(MAKE) -s test-edges BUFFER_SIZE=$$bs; \
		echo "Running valgrind on tests/test_edges"; \
		valgrind --leak-check=full --show-leak-kinds=all ./tests/test_edges; \
		echo "=== BUFFER_SIZE=$$bs: more tests ==="; \
		$(MAKE) -s test-more BUFFER_SIZE=$$bs; \
	done

clean:
	@rm -f $(BIN)

fclean: clean

re: fclean all

.PHONY: all test valgrind clean fclean re
.PHONY: test-edges test-all
.PHONY: test-more valgrind-all
