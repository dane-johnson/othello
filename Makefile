CXXFLAGS = -g

.PHONY: all
all: othello gui
othello: board.o attacks.o move_generation.o main.o
	$(CXX) $(CXXFLAGS) -o $@ $^
gui: gui.scm
	chicken-csc -static gui.scm
