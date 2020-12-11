CXXFLAGS = -g

.PHONY: all
all: othello
othello: board.o attacks.o move_generation.o main.o
	$(CXX) $(CXXFLAGS) -o $@ $^
