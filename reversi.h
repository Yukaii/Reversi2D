#pragma once
#define SIZE 8
#define MAX 64
enum eSTATE{eWHITE = -1, eEMPTY = 0, eBLACK = 1};

class Reversi
{
public:
	int getWhite(){
		return numWhite;
	}

	int getBlack(){
		return numBlack;
	}

	//reset the board array and variables
	bool reset();

	//return now who should place
	bool BW(){
		if (which == eBLACK) return true;
		else return false;
	}
	
	//place B/W
	bool placeHere(int x, int y);

	//check if it's end
	bool isEnd();

	//other action
	bool undo();
	bool redo();

	Reversi();
	~Reversi();

private:
	//the number of Black and White
	int numWhite, numBlack;

	//B/W's turn
	eSTATE which;

	//B->W, W->B
	void flip(){
		if (which == eBLACK) which = eWHITE;
		else which = eBLACK;
	}

	//decide whether can place B/W
	bool decided(int x, int y);
	
	bool walkaround(int x, int y, int _x, int _y,int oppcolor);
	void setaround();
	void setBW(int x, int y, int color){
		board[x + y * SIZE] = color;
	}
	int getBW(int x, int y){
		return board[x + y * SIZE];
	}
	bool checkZ();


	//store board info
	eSTATE board[SIZE * SIZE];

	//scan board info
	void scanBW();

	bool endflag;

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

