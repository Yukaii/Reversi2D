#include "reversi_new.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

//mark dir if can eat B/W
int mark[9];

Reversi::Reversi(){
	for(int j = 0; j < SIZE; j++)
	{
		for(int i = 0; i < SIZE; i++)
		{
			board[i + j*8] = eEMPTY;
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

	int center = 0 + SIZE/2 - 1;
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

	//end control
	endflag = false;
}

bool Reversi::decided(int x, int y){
	for (int i = 0; i < 9; i++)
	{
		mark[i] = 0;
	}	

	int mycolor = eBLACK, oppcolor = eWHITE;
	if (which == eWHITE)
	{
		mycolor = -1;
		oppcolor = 1;
	}
	int i = 0;

	for (int _y = -1; _y <= 1; _y++){
	for (int _x = -1; _x <= 1; _x++){
		while(x+_x < SIZE &&  x+_x >= 0 && y＋_y < SIZE && y＋_y >= 0)
		{
			if (getBW(x+_x, y＋_y) == oppcolor)
			{
				if (walkaround(x+_x, y＋_y, _x, _y,oppcolor))
				{
					mark[i] = 1;
				}
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
	int mycolor = (-1)＊oppcolor;

	while(x < SIZE &&  x >= 0 && y < SIZE && y >= 0){
		if (getBW(x+_x, y＋_y) == mycolor){
			return true;
		}
		else if (getBW(x+_x, y＋_y) == oppcolor)
			return walkaround(x+_x, y＋_y, _x, _y,oppcolor);
		//empty
		else break;
	}
	return false
}

bool Reversi::placeHere(int x, int y)
{
	int mycolor = which;
	int oppcolor = (-1) * which;
	int ori_x = x;
	int ori_y = y;

	if (decided(x, y) && getBW(x, y) == eEMPTY)
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

	flip();
	push();
	step++;
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


