#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include <stdio.h>
#include <string>
#include <sstream>
#include "reversi.cpp"
//#include "LTexture.cpp"


bool init();
//grid size = 50 * 50 * 8 * 8 = 400 * 400
const int SCREEN_WIDTH = 520;
const int SCREEN_HEIGHT = 450;

//AI toggle bar
const int TOGGLE_WIDTH = 60;
const int TOGGLE_HEIGHT = 30;

//disks size
const int DISK_LEN = 40;

const int GRID_SIZE = 50;

const int HINT_LEN = 30;

enum DiskSprite
{
	DISK_BLACK = 0,
	DISK_BLACK_TRANSPARENT = 1,
	DISK_WHITE = 2,
	DISK_WHITE_TRANSPARENT = 3,
	DISK_TOTAL = 4
};

SDL_Rect DiskSpriteClips[DISK_TOTAL];
//LTexture DiskTexture;

SDL_Surface* gSurface = NULL;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

SDL_Texture* black = NULL;
SDL_Texture* white = NULL;
SDL_Texture* undo  = NULL;
SDL_Texture* redo  = NULL;
SDL_Texture* check = NULL;
SDL_Texture* uncheck = NULL;
SDL_Texture* hintb = NULL;
SDL_Texture* hintw = NULL;

SDL_Texture* textureText = NULL;
int mWidth;
int mHeight;

////Text color we used
SDL_Color cWhite = {255, 255, 255};
SDL_Color cBlack = {0, 0, 0};

/////////////////////////
/// REVERSI COMPONENT ///
/////////////////////////
Reversi board;

int hint[SIZE][SIZE];
bool hint_flag = false;


void render(int x, int y, int w, int h, SDL_Texture* texture){
	SDL_Rect rec = {x, y, w, h}; 
	SDL_RenderCopy(gRenderer, texture, NULL, &rec);
}

bool init(){
	bool success = true;
	TTF_Init();
	gFont = TTF_OpenFont("DroidSans.ttf", 28);

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

		gWindow = SDL_CreateWindow("Reversi Board", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);//rgba

			gSurface = SDL_GetWindowSurface(gWindow);
		}
	}


	return success;
}


SDL_Texture* LoadImage(std::string path){
	SDL_Surface *loaded = IMG_Load( path.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, loaded);
	SDL_FreeSurface(loaded);
	return texture;
}

void LoadMedia()
{
	black = LoadImage("res/circle_b.png");
	white = LoadImage("res/circle_w.png");
	check = LoadImage("res/checkbox_checked.png");
	uncheck = LoadImage("res/checkbox_unchecked.png");
	undo = LoadImage("res/undo.png");
	redo = LoadImage("res/redo.png");
	hintb = LoadImage("res/hint_b.png");
	hintw = LoadImage("res/hint_w.png");
}

SDL_Texture* LoadText(std::string textureText, SDL_Color textColor){
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	SDL_Texture* temp;
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        temp = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( temp == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	return temp;
}

void close()
{
	SDL_FreeSurface(gSurface);
	gSurface = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

void RenderGrid()
{
	int offset = (GRID_SIZE - DISK_LEN)/2;
	for (int j = 0; j < 8; j++){
	for (int i = 0; i < 8; i++){
		int x = i * 400/8;
		int y = j * 400/8;
		if (board.getBW(i, j) == eWHITE) render(x+offset, y+offset, DISK_LEN, DISK_LEN, white);
		if (board.getBW(i, j) == eBLACK) render(x+offset, y+offset, DISK_LEN, DISK_LEN, black);
	
	}
	}	
}


void handleEvent(SDL_Event *event){
	
	//If mouse event happened
	if(event->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		int grid_region_x,grid_region_y;
		SDL_GetMouseState( &x, &y );

		for (int j = 0; j < 8; j++){			
		for (int i = 0; i < 8; i++){

			grid_region_x = i *  GRID_SIZE;
			grid_region_y = j * GRID_SIZE;

			if (x < grid_region_x + GRID_SIZE && x > grid_region_x 
			 && y < grid_region_y + GRID_SIZE && y > grid_region_y)
			{
				board.placeHere(i, j);	
			}

		}
		}

		if (x < 310 && x > 280 && y > 410 && y < 430) board.undo();
		if (x < 365 && x > 335 && y > 410 && y < 430) board.redo();
		//render(280, 410, 30, 30, undo);
		//render(335, 410, 30, 30, redo);

		if (x < 430 && x > 410 && y < 60 && y > 40) hint_flag = !hint_flag;
		//render(410, 40, 20, 20, chk);

	}
}

void RenderFooter(){
	//draw footer
	SDL_SetRenderDrawColor(gRenderer, 70, 194, 157, 255);
	SDL_Rect footer = {0, 400, SCREEN_WIDTH, SCREEN_HEIGHT-400};
	SDL_RenderFillRect(gRenderer, &footer);						

	ostringstream bbb, www, star;
	string info;
	bbb << ": " << board.getBlack();
	www << ": " << board.getWhite();
	star << "*";

	info = star.str();
	textureText = LoadText(info, cBlack);
	if (board.BW()) {
		render(23, 410, mWidth, mHeight, textureText);
	}

	info = bbb.str();
	textureText = LoadText(info, cWhite);
	render(40, 410, 30, 30, black);
	render(80, 410, mWidth, mHeight, textureText);

	info = star.str();
	textureText = LoadText(info, cBlack);
	if (!board.BW()) {
		render(153, 410, mWidth, mHeight, textureText);
	}	

	info = www.str();
	textureText = LoadText(info, cWhite);
	render(170, 410, 30, 30, white);
	render(210, 410, mWidth, mHeight, textureText);

	render(280, 410, 30, 30, undo);
	render(335, 410, 30, 30, redo);
}

void RenderSidebar(){
	//draw right sidebar #00FE87
	SDL_SetRenderDrawColor(gRenderer, 70, 194, 157, 255);
	SDL_Rect Rsidebar = {400, 0, SCREEN_WIDTH-400, SCREEN_HEIGHT};
	SDL_RenderFillRect(gRenderer, &Rsidebar);

	SDL_Texture* chk = (hint_flag) ? check : uncheck;
	render(410, 40, 20, 20, chk);

	ostringstream hint;
	hint << "HINT";
	string hnt = hint.str();
	textureText = LoadText(hnt, cWhite);
	render(443, 35, mWidth, mHeight, textureText);

}

bool endProcess(){
	if (!board.haveNextMove()){
		board.flip();		

		if (!board.haveNextMove()){
			return true;
		}
	}
	return false;
}

void renderHint(SDL_Event* event){
	for(int j = 0; j < 8; j++){
	for(int i = 0; i < 8; i++){
		if (board.decided(i, j)) hint[i][j] = 1;
	}
	}
	if (event->type == SDL_MOUSEMOTION){	
		int x, y;
		SDL_GetMouseState( &x, &y );	
		int offset = (GRID_SIZE - HINT_LEN)/2;

		for(int j = 0; j < 8; j++){
		for(int i = 0; i < 8; i++){	
			SDL_Texture* tHint = board.BW() ? hintb : hintw;
			int hint_x = i * 50;
			int hint_y = j * 50;
			if (x < 400 && x > 0 && y < 400 && y > 0)
			{
				if (hint[i][j] == 1)
				render(hint_x + offset, hint_y + offset, HINT_LEN, HINT_LEN, tHint);
			}

		}
		}
	}

	for(int j = 0; j < 8; j++){
	for(int i = 0; i < 8; i++){
		hint[i][j] = 0;
	}
	}	

}

void WaitReset(){
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0){
		if (event.type == SDLK_RETURN || event.type == SDLK_RETURN2 || event.type == SDL_QUIT) break;
		else continue;
	}
	board.reset();	
}


int main(int argc, char* args[])
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
				else if (event.type == SDLK_RETURN || event.type == SDLK_RETURN2)
					board.reset();

				handleEvent(&event);
			}
			//sort of green 27, 129, 62, 255

			
			//draw background color 
			SDL_SetRenderDrawColor(gRenderer, 179, 248, 221, 255);
			SDL_Rect fullScreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			SDL_RenderFillRect(gRenderer, &fullScreen);
			

			
			//Draw Board grid
			SDL_SetRenderDrawColor(gRenderer, 175, 175, 175, 255);
			for (int i = 0; i <= 8; i++)
			{
				SDL_RenderDrawLine(gRenderer, i*50, 0, i*50, SCREEN_HEIGHT);
				SDL_RenderDrawLine(gRenderer, 0, i*50, SCREEN_WIDTH, i*50);
			}

			//////////////////////////
			///  RENDER COMPONENT  ///
			//////////////////////////

			RenderSidebar();							
			
			RenderFooter();			

			////////////////////////
			///   GAME CONTROL   ///
			////////////////////////
			
			if (endProcess())
			{
				ostringstream winflag;
				SDL_Texture* who = (board.getWhite() < board.getBlack()) ? black : white;
				if (board.getWhite() == board.getBlack()) who = NULL;
				render(443, 370, 30, 30, who);
				//else cout << "tie......!" << endl;
				if (board.getWhite() != board.getBlack()) winflag << "WIN!";
													else  winflag << "TIE!";

				string win = winflag.str();
				textureText = LoadText(win, cWhite);
				render(430, 410, mWidth, mHeight, textureText);

				SDL_RenderPresent(gRenderer);				

				while(SDL_PollEvent(&event) != 0){
					ostringstream msgg;
					msgg << "Enter to Continue";
					string msg = msgg.str();
					textureText = LoadText(msg, cWhite);
					render(443, 410, mWidth/2, mHeight/2, textureText);
					SDL_RenderPresent(gRenderer);
					WaitReset();				
				}	

			}
 
			//board.activateAI(eBLACK, true);
			//board.activateAI(eWHITE, true);
			//board.AIMove();

			RenderGrid();

			if (hint_flag) renderHint(&event);

			SDL_RenderPresent(gRenderer);
			
			//SDL_UpdateWindowSurface(gWindow);
		}
	}	
	close();
	return 0;
}
