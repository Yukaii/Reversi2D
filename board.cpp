#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

#include <stdio.h>
#include <string>
#include "reversi.cpp"
//#include "circle.cpp"

#define MAX 8


bool init();

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;

SDL_Surface* bSurface = NULL;
SDL_Window* bWindow = NULL;
SDL_Renderer* bRenderer = NULL;

SDL_Texture* black = NULL;
SDL_Texture* white = NULL;


void render(int x, int y, int w, int h, SDL_Texture* texture){
	SDL_Rect rec = {x, y, w, h}; 
	SDL_RenderCopy(bRenderer, texture, NULL, &rec);
}

bool init(){
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}		

		bWindow = SDL_CreateWindow("Reversi Board", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if( bWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			bRenderer = SDL_CreateRenderer(bWindow, -1, SDL_RENDERER_ACCELERATED);
			
			SDL_SetRenderDrawColor(bRenderer, 255, 255, 255, 255);//rgba

			bSurface = SDL_GetWindowSurface(bWindow);
		}
	}


	return success;
}


SDL_Texture* LoadBW(std::string path){
	SDL_Surface *loaded = IMG_Load( path.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(bRenderer, loaded);
	SDL_FreeSurface(loaded);
	return texture;
}

void LoadMedia()
{
	black = LoadBW("circle_b.png");
	white = LoadBW("circle_w.png");
}




void close()
{
	SDL_FreeSurface(bSurface);
	bSurface = NULL;

	SDL_DestroyWindow(bWindow);
	bWindow = NULL;

	SDL_Quit();
}


int main( int argc, char* args[])
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		LoadMedia();
		bool quit = false;
		SDL_Event event;
		//Main Loop while application running
		while(!quit)
		{
			while(SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
					quit = true;
			}

			//sort of green 27, 129, 62, 255
			
			
			//draw background color
			SDL_SetRenderDrawColor(bRenderer, 235, 255, 235, 0);
			SDL_Rect fullScreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			SDL_RenderFillRect(bRenderer, &fullScreen);
			

			
			//Draw Board grid
			//each grid width = 50, height = 50
			SDL_SetRenderDrawColor(bRenderer, 0, 0, 0, 240);
			for (int i = 0; i <= 8; i++)
			{
				SDL_RenderDrawLine(bRenderer, i*50, 0, i*50, SCREEN_HEIGHT);
				SDL_RenderDrawLine(bRenderer, 0, i*50, SCREEN_WIDTH, i*50);
			}

			render(5, 5, 40, 40, black);		
			
			SDL_RenderPresent(bRenderer);
			
			//SDL_UpdateWindowSurface(bWindow);
		}
	}	
	close();
	return 0;
}
