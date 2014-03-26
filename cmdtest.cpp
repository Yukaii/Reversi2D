#include "reversi.cpp"
#include <iostream>
#include <stdlib.h>
#include "getch.cpp"
using namespace std;


Reversi rev;

void Print(){
	system("clear");
	rev.print();

	cout << "now: "	;
	if (rev.BW()) cout << "x" << endl;
		else cout << "o" << endl;

	cout << endl;
	cout << "x: " << rev.getBlack() << ", o: " << rev.getWhite() << endl << endl;	
}

bool endProcess(){
	if (!rev.haveNextMove()){
		rev.flip();		
		Print();

		if (!rev.haveNextMove()){
			return true;
		}
	}
	return false;
}

				
int main(int argc, char *argv[]){
	
	rev.reset();
	
	int x, y;
	char ans;
	
	while(1)
	{
		//rev.activateAI(eBLACK, true);
		//rev.activateAI(eWHITE, true);
		Print();

		if (endProcess())
		{
			if     (rev.getWhite() > rev.getBlack()) cout << "White win!!!" << endl;
			else if(rev.getWhite() < rev.getBlack()) cout << "Black win!!!" << endl;	
			else cout << "tie......!" << endl;

			cout << "Press ENTER to continue";

			fgetc(stdin);
			fgetc(stdin);
			rev.reset();
			continue;
		}

		if (rev.haveNextMove()){
			if(rev.AIMove()) continue;
		}

		cin >> x >> y;
	
		if (x <= 7 && x >= 0 && y <= 7 && y >= 0 ){
			rev.placeHere(x, y);
		}

		else if (x == 9) rev.undo();
		else if (x == 10) rev.redo();
	}
	
}