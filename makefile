CC = g++
FLAGS = $(shell sdl2-config --libs) -lSDL2main -lSDL2_image

SOURCE = main.cpp

OBJS = $(SOURCE:.cpp=.o)

reversi: $(OBJS)
	$(CC) -o $@ $(OBJS) $(FLAGS)

clean:
	rm -r *.o && rm reversi

