CC = c++

CXXFLAGS = -Werror -Wall -std=c++17 -stdlib=libc++

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = bin
PROGRAM = exp_eval

.PHONY: all clean

all: dirs libs game

dirs:
	mkdir -p ./$(BIN)

libs:


run: clean all
	$(BIN)/$(PROGRAM)

game: $(OBJ)
	$(CC) -o $(BIN)/$(PROGRAM) $^

%.o: %.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)