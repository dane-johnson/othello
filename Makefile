CXXFLAGS = -g

.PHONY: all clean
all: othello
othello: board.o attacks.o move_generation.o main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

gui: gui.scm
	chicken-csc -static gui.scm

clean:
	rm othello
	rm *.o