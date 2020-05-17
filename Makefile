CXX = g++

CXXFLAGS = `pkg-config --cflags --libs sdl2` -lSDL2_mixer -Wall -std=c++17 -Wextra

SRCS = main.cpp spong.cpp ball.cpp

OBJS = $(SRCS:.cpp=.o)

spong: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(OBJS)
