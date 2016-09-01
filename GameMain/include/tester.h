#pragma once

#include "game_board.h"
#include "chessbox.h"

class Tester {
public:
	static void putAllChessAtMap(int pid, int x = 19, int y = 19) {
		GameBoard gb;
		gb.showInScreen();
		printf("\n");
		ChessBox cb;
		vector<Point> pos;
		pos.push_back({ x, y });
		size_t posl;
		Chessman::FormSet fs;
		for (ChessBox::ChessIter chess_iter = cb.begin(); chess_iter != cb.end(); ) {
			size_t plen = pos.size();
			size_t i;
			for (i = 0; i < plen; ++i) {
				fs = gb.getValidForms(pid, *chess_iter, pos[i].x, pos[i].y);
				if (!fs.empty()) {
					break;
				}
			}
			if (fs.empty()) {
				++chess_iter;
				continue;
			}
			gb.putChessAt(pid, *chess_iter, fs.begin(), pos[i].x, pos[i].y);
			cb.removeChess(chess_iter);
			chess_iter = cb.begin();
			gb.showInScreen();
			printf("\n");
			pos = gb.getAvailablePoints(pid);
			gb.showPosInScreen(pid, pos);
			printf("%d\n", cb.getChessNum());
		}
	}
};