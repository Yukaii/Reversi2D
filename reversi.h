#pragma once
//the width/height of the board
//must be even number
#define SIZE 4

//max trace back steps(history steps remember)
//smaller than SIZE^2 is fine
#define MAX 64
const int eWHITE = -1, eEMPTY = 0, eBLACK = 1;

class Reversi
{
public:
	/////////////////
	/// VARIABLES ///
	/////////////////

	int getWhite(){return numWhite;}
	int getBlack(){return numBlack;}

	//return now who should place
	bool BW(){
		if (which == eBLACK) return true;
			else return false;
	}	

	//get x, y
	int getBW(int x, int y){
		return board[x + y * SIZE];
	}	

	/////////////////////////
	/// CONTROL FUNCTIONS ///
	/////////////////////////

	//place B/W
	bool placeHere(int x, int y);

	//check if there's next move
	bool haveNextMove();

	//activate AI for specific color
	//return false if the color has been activated
	//param = true:activate, param = false:disactivate
	void activateAI(int color, bool param);

	//A random simple ai...less is more >///<
	bool AIMove();

	//Black to white/ white to Black
	void flip(){
		if (which == eBLACK) which = eWHITE;
		else which = eBLACK;
	}	

	//other action
	void undo();
	void redo();
	void print();
	bool reset();	

	Reversi();
	~Reversi();

private:
	////////////////////////////////
	//    BOARD VAR & ACTIONS    ///
	////////////////////////////////

	//the number of Black and White
	int numWhite, numBlack;

	//whose turn, eBLACK or eWHITE
	int which;

	bool AIflagB;
	bool AIflagW;

	//decide whether can place B/W
	bool decided(int x, int y);
	
	//go find opposite color recursively
	bool walkaround(int x, int y, int _x, int _y,int oppcolor);

	//set board(x, y) to color(empty/black/white)
	void setBW(int x, int y, int color){
		board[x + y * SIZE] = color;
	}
	//check if board is empty
	bool checkZ();

	//store board info
	int board[SIZE * SIZE];

	//scan board info
	void scanBW();

	////////////////////////////
	//        HISTORY         //
	////////////////////////////
	int history[SIZE*SIZE][MAX];

	//history index
	int index;

	//history tale
	int tale;

	//total step you make
	int step;

	//push board status history
	void push();
	//////END HISTORY CONTROL//////
};

