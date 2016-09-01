#include <iostream>
using std::cin; using std::cout; using std::endl;

#include "chessbox.h"
#include "game_board.h"

int main()
{
	GameBoard gb;
	gb.showInScreen();
	printf("\n");
	ChessBox cb;
	for (ChessBox::ChessIter chess_iter = cb.begin(); chess_iter != cb.end(); ++chess_iter) {
		printf("all forms:\n");
		for (Chessman::FormIter form_iter = (*chess_iter)->begin(); form_iter != (*chess_iter)->end(); ++form_iter) {
			Chessman::showInScreen(*form_iter);
			printf("\n");
		}
		printf("fit forms:\n");
		Chessman::FormSet fs = gb.getValidForms(1, *chess_iter, 19, 19);
		for (Chessman::FormIter form_iter = fs.begin(); form_iter != fs.end(); ++form_iter) {
			Chessman::showInScreen(*form_iter);
			printf("\n");
		}
		gb.putChessAt(1, *chess_iter, fs.begin(), 19, 19);
		gb.showInScreen();
		vector<Point> res = gb.getAvailablePoints(1);
		gb.showPosInScreen(1, res);
		cin.get();
	}
	cin.get();
}