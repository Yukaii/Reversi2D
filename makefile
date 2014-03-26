CC = g++
FLAGS = $(shell sdl2-config --libs) -lSDL2main -lSDL2_image

SOURCE = board.cpp

OBJS = $(SOURCE:.cpp=.o)

board: $(OBJS)
	$(CC) -o $@ $(OBJS) $(FLAGS)

clean:
	rm -r *.o && rm board

test:
	
