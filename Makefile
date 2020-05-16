CXX = g++

CXXFLAGS = `pkg-config --cflags --libs sdl2` -ggdb3 -Wall -std=c++17 -Wextra

SRCS = main.cpp spong.cpp ball.cpp

OBJS = $(SRCS:.cpp=.o)

game: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(OBJS)
