Reversi 2D Game - in SDL Library
================================

# About
A 2D Reversi Game, using SDL(Simple DirectMedia Layer)


## Project Website: 

Go to [Bitbucket](https://bitbucket.org/yukaihuang1993/reversi-the-game-written-in-sdl-library)

# Preview
![GamePlay](https://lh6.googleusercontent.com/-2US_kYp6wgM/UzUf6TGeUSI/AAAAAAAAIuo/8VCPuff78mU/w522-h481-no/1.png)

![With Hing](https://lh4.googleusercontent.com/-6UrGrmTQkG8/UzUf6NY3tHI/AAAAAAAAIug/W07h5AJXXHQ/w522-h481-no/2.png)

# Prequisite
*	[SDL2 runtime Library](http://www.libsdl.org/download-2.0.php)

Download from [official website](http://www.libsdl.org) and extract *.dll files to the game folder.

# Feature

*	Playable UI
*	AI(poor XDD)


## Eastern Egg
enter "↑↑↓↓←→←→BA" and see what happen:





......nothing happen

# Installation
### Linux
Download and Setup Develpment Libraries: 

*	[SDL2](http://www.libsdl.org/download-2.0.php)
*	[SDL2 image](https://www.libsdl.org/projects/SDL_image/)
*	[SDL2 ttf](https://www.libsdl.org/projects/SDL_ttf/)

use following command for compilation.

	make

### Windows
Install MinGW and Setup SDL Develpment Library for mingw.

then Use CMD to compile yourself:

	mingw32-g++ -c main.cpp -o reversi.o
	mingw32-g++ reversi.o -mwindows -o reversi.exe -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2

BTW, it's still buggy and memory wastes, but just "playable".


# Play

## Rule

Look up from [here](http://en.wikipedia.org/wiki/Reversi)

# Todo List

*	multiple color scheme
*	Eastern eggs
*	About popup
