CXXFLAGS = -g -O3 -fopencilk
LIBS = -ltbb

CXX = ~/build/bin/clang++
SRCS = $(wildcard *.cpp)
HDRS = $(wildcard *.hpp) $(wildcard ./agents/*.hpp)
OBJS = $(patsubst %.cpp,./build/%.o,$(SRCS))
AGNT_SRCS = $(wildcard ./agents/*.cpp)
AGNT_OBJS = $(patsubst %.cpp,%.o,$(AGNT_SRCS))

.PHONY: all clean
all: othello

build:
	mkdir -p build

agents/%.o: agents/%.cpp $(HDRS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

build/%.o: %.cpp $(HDRS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

othello: $(OBJS) $(AGNT_OBJS)
	$(CXX) $(LIBS) $(CXXFLAGS) -o $@ $^

gui: gui.scm
	chicken-csc -static gui.scm

clean:
	rm -f othello build/*.o agents/*.o
