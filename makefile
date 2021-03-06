CC = g++
FLAGS = $(shell sdl2-config --libs) -lSDL2main -lSDL2_image -lSDL2_ttf

SOURCE = main.cpp

OBJS = $(SOURCE:.cpp=.o)

reversi: $(OBJS)
	$(CC) -o $@ $(OBJS) $(FLAGS)

clean:
	rm -r *.o && rm reversi cmdtest

test: cmdtest.o
	g++ cmdtest.cpp -o cmdtest

