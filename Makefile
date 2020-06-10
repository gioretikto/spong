CXX = g++

CXXFLAGS = `pkg-config --cflags --libs sdl2` -lSDL2_mixer -lSDL2_ttf -Wall -std=c++17 -Wextra

ifeq ($(SYSTEM), SunOS)
	CFLAGS+= -D__EXTENSIONS__
endif

SRCS = main.cpp spong.cpp ball.cpp

OBJS = $(SRCS:.cpp=.o)

spong: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(OBJS)
