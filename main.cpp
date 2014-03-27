#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

#include <stdio.h>
#include <string>
#include "reversi.cpp"


bool init();
//grid size = 50 * 50 * 8 * 8 = 400 * 400
const int SCREEN_WIDTH = 520;
const int SCREEN_HEIGHT = 450;

SDL_Surface* bSurface = NULL;
SDL_Window* bWindow = NULL;
SDL_Renderer* bRenderer = NULL;

SDL_Texture* black = NULL;
SDL_Texture* white = NULL;


/////////////////////////
/// REVERSI COMPONENT ///
/////////////////////////
Reversi board;


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
	black = LoadBW("res/circle_b.png");
	white = LoadBW("res/circle_w.png");
}

void close()
{
	SDL_FreeSurface(bSurface);
	bSurface = NULL;

	SDL_DestroyWindow(bWindow);
	bWindow = NULL;

	SDL_Quit();
}

void RenderGrid()
{
	int x_offset = 5, y_offset = 5;
	for (int j = 0; j < 8; j++){
	for (int i = 0; i < 8; i++){
		int x = i * 400/8;
		int y = j * 400/8;
		if (board.getBW(i, j) == eWHITE) render(x+x_offset, y+y_offset, 40, 40, white);
		if (board.getBW(i, j) == eBLACK) render(x+x_offset, y+y_offset, 40, 40, black);
	}
	}

	
	
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
			SDL_SetRenderDrawColor(bRenderer, 179, 248, 221, 255);
			SDL_Rect fullScreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			SDL_RenderFillRect(bRenderer, &fullScreen);
			

			
			//Draw Board grid
			//each grid width = 50, height = 50 #45C29D
			SDL_SetRenderDrawColor(bRenderer, 175, 175, 175, 255);
			for (int i = 0; i <= 8; i++)
			{
				SDL_RenderDrawLine(bRenderer, i*50, 0, i*50, SCREEN_HEIGHT);
				SDL_RenderDrawLine(bRenderer, 0, i*50, SCREEN_WIDTH, i*50);
			}

			//draw right sidebar #00FE87
			SDL_SetRenderDrawColor(bRenderer, 70, 194, 157, 255);
			SDL_Rect Rsidebar = {400, 0, SCREEN_WIDTH-400, SCREEN_HEIGHT};
			SDL_RenderFillRect(bRenderer, &Rsidebar);

			//draw footer
			SDL_SetRenderDrawColor(bRenderer, 70, 194, 157, 255);
			SDL_Rect footer = {0, 400, SCREEN_WIDTH, SCREEN_HEIGHT-400};
			SDL_RenderFillRect(bRenderer, &footer);						
				
			
			RenderGrid();

			SDL_RenderPresent(bRenderer);
			
			//SDL_UpdateWindowSurface(bWindow);
		}
	}	
	close();
	return 0;
}
