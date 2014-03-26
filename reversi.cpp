#include "reversi.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

using namespace std;

//mark dir if can eat B/W
int mark[9];

Reversi::Reversi(){
	for(int j = 0; j < SIZE; j++)
	{
		for(int i = 0; i < SIZE; i++)
		{
			setBW(i, j, eEMPTY);
		}
	}
	reset();
}

Reversi::~Reversi(){
	delete[] &board;
	delete[] &history;
}


bool Reversi::reset(){

	for(int i = 0; i < SIZE * SIZE; i++)
	{
		//set all to empty
		board[i] = eEMPTY;
		for (int j = 0; j < MAX; j++)
		{
			history[i][j] = eEMPTY;
		}
	}

	int center = ceil(SIZE/2) - 1;
	//initialize board status
	board[center     + center    *SIZE] = eWHITE;
	board[(center+1) + (center+1)*SIZE] = eWHITE;
	board[(center+1) + center    *SIZE] = eBLACK;
	board[center     + (center+1)*SIZE] = eBLACK;

	numBlack = 2;
	numWhite = 2;

	//black first
	which = eWHITE;

	//history param
	index = 0;
	tale = 0;
	step = 0;
	push();

	AIflagB = false;
	AIflagW = false;
}

//print the board in command line interface
void Reversi::print(){
	cout << endl;
	for (int j = 0; j < SIZE; j++){		
		cout << "  ";
		for (int i = 0; i < SIZE; i++){
			if (getBW(i, j) == eEMPTY) cout << "- ";
			if (getBW(i, j) == eBLACK) cout << "x ";
			if (getBW(i, j) == eWHITE) cout << "o ";
		}
		cout << j << endl;
	}
	cout << "  ";
	for (int i = 0; i < SIZE; i++) cout << i << " ";
	cout << endl << endl;
	
	//count black/white on the board again
	scanBW();
}
void Reversi::activateAI(int color, bool param){
	if (param)
	{
		if (color == eBLACK) AIflagB = true;
		if (color == eWHITE) AIflagW = true;
	}
	else
	{
		if (color == eBLACK) AIflagB = false;
		if (color == eWHITE) AIflagW = false;		
	}
}

bool Reversi::AIMove(){
	if ((AIflagB == true && which == eBLACK) || (AIflagW == true && which == eWHITE))
	{
		srand(time(NULL));
		
		int move_number = 0;
		for(int j = 0; j < SIZE; j++){
			for (int i = 0; i < SIZE; i++){
				if(getBW(i, j) == eEMPTY && decided(i, j)) move_number++;
			}
		}
		if (move_number == 0) return false;
		


		//pick up random move
		int witchcraft = rand()%move_number;

		int k = 0;
		for(int j = 0; j < SIZE; j++){
			for (int i = 0; i < SIZE; i++){
				if(getBW(i, j) == eEMPTY && decided(i, j)) k++;
				if (k == witchcraft)
				{
					if (placeHere(i, j)) return true;
				}
			}
		}
	}
	else return false;

}

bool Reversi::haveNextMove(){
	for (int j = 0; j < SIZE; j++){
		for (int i = 0; i < SIZE; i++){
			if (getBW(i, j) == eEMPTY)
			{
				if (decided(i, j)) return true;
			}
		}
	}
	return false;
}


bool Reversi::decided(int x, int y){
	if (getBW(x, y) != eEMPTY) return false;

	for (int i = 0; i < 9; i++)
	{
		mark[i] = 0;
	}	

	int mycolor = eBLACK, oppcolor = eWHITE;
	if (which == eWHITE)
	{
		mycolor =  eWHITE;
		oppcolor = eBLACK;
	}

	int i = 0;
	for (int _y = -1; _y <= 1; _y++){
	for (int _x = -1; _x <= 1; _x++){
		int next_x = x + _x;
		int next_y = y + _y;
		if(next_x < SIZE && next_x >= 0 && next_y < SIZE && next_y >= 0 && getBW(next_x, next_y) == oppcolor)
		{
			if (walkaround(next_x, next_y, _x, _y,oppcolor))
			{
				mark[i] = 1;
			}
		}
		i++;
	}
	}

	for (int k = 0; k < 9; k++) if (mark[k] == 1) return true;
	
	return false;

}

bool Reversi::walkaround(int x, int y, int _x, int _y,int oppcolor)
{
	int mycolor = -oppcolor;

	while(x < SIZE &&  x >= 0 && y < SIZE && y >= 0){
		int next_x = x + _x;
		int next_y = y + _y;		
		if (getBW(next_x, next_y) == mycolor){
			return true;
		}
		else if (getBW(next_x, next_y) == oppcolor)
			return walkaround(next_x, next_y, _x, _y,oppcolor);
		//if empty
		else return false;
	}
	return false;
}

bool Reversi::placeHere(int x, int y)
{
	int mycolor = which;
	int oppcolor = (-1) * which;
	int ori_x = x;
	int ori_y = y;

	if (getBW(x, y) == eEMPTY && decided(x, y))
	{
		setBW(x, y, which);
		
		int i = 0;
		for (int _y = -1; _y <= 1; _y++){
		for (int _x = -1; _x <= 1; _x++){
			x = ori_x;
			y = ori_y;

			if (mark[i]!=0)
			{
				while(getBW(x+_x, y+_y) != mycolor)
				{
					setBW(x+_x, y+_y, mycolor);
					x += _x;
					y += _y;
				}	
				//end while			
			}
			//end if
			i++;
		}
		}	
		//end for	
	}
	else return false;

	flip();
	push();
	step++;
	return true;
}

void Reversi::scanBW(){
	int B = 0;
	int W = 0;
	for (int j = 0; j < SIZE; j++) {
		for (int i = 0; i < SIZE; i++){
			if (getBW(i, j) == eBLACK) B++;
			if (getBW(i, j) == eWHITE) W++;
		}
	}
	numBlack = B;
	numWhite = W;
}

void Reversi::undo(){
	int i, j; 
	if(index > 0 && step > 0) {
		index--;
		step--;	
		for (j = 0 ; j < SIZE; j++){
			for (i = 0; i < SIZE; i++){
				board[i + j*SIZE] = history[i + j*SIZE][index];
			}
		}
		flip();
	}	
}

void Reversi::redo(){
	int i, j;
	if(index < tale && checkZ()) {
		index++;
		step++;
		for (j = 0; j < SIZE; j++){
			for (i = 0; i < SIZE; i++){
				board[i + j*SIZE] = history[i + j*SIZE][index];
			}
		}
		flip();
	}
}

bool Reversi::checkZ() {
	for (int i = 0; i < SIZE*SIZE; i++)
		if (history[i][index+1] != 0) return true;
	return false;
}

void Reversi::push(){
	int i, j, k;
	if (index == MAX-1)
	{
		for (i = 0; i < MAX-1; i++)
		{
			for (k = 0; k < SIZE*SIZE; k++){
				//move all histroy left by 1
				history[k][i] = history[k][i+1];
			}
		}
		for (j = 0; j < SIZE; j++){
			for (i = 0; i < SIZE; i++){
				history[i + j*SIZE][index] = board[i + j*SIZE];
			}
		}
	}
	else if (index < MAX-1) 
	{
		index++;
		for (j = 0; j < SIZE; j++){
			for (i = 0; i < SIZE; i++){
				history[i + j*SIZE][index] = board[i + j*SIZE];

				//refresh new history steps inorder result in chaos(?)
				if (index+1 < MAX) history[i + j*SIZE][index+1] = 0;
			}
		}
		tale++;
	}
}


