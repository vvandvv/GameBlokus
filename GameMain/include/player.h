#pragma once

#include "game_board.h"
#include "chessbox.h"

class Player {
public:
	int player_id;
	GameBoard *mGameBoard;
	ChessBox *mChessBox;
public:
	void putChess(int hn) const {
		mGameBoard->showInScreen();
		printf("\n");
		vector<Point> pos = mGameBoard->getAvailablePoints(player_id);
		size_t posl;
		Chessman::FormSet fs;
		for (ChessBox::ChessIter chess_iter = mChessBox->begin(); chess_iter != mChessBox->end(); ) {
			size_t plen = pos.size();
			size_t i;
			for (i = 0; i < plen; ++i) {
				fs = mGameBoard->getValidForms(player_id, *chess_iter, pos[i].x, pos[i].y);
				if (!fs.empty()) {
					break;
				}
			}
			if (fs.empty()) {
				++chess_iter;
				continue;
			}
			mGameBoard->putChessAt(player_id, *chess_iter, fs.begin(), pos[i].x, pos[i].y);
			mChessBox->removeChess(chess_iter);
			mGameBoard->showInScreen();
			printf("\n");
			break;
		}
	}
};