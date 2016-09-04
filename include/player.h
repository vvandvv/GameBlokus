#pragma once

#include "game_board.h"
#include "chessbox.h"

class Player {
	ChessBox *const chess_box = new ChessBox();
	const int player_id;
	const Point birth_place;
public:
	GameBoard *mGameBoard;
public:
	Player(int pid, GameBoard *gb, const Point &pt) : player_id(pid), mGameBoard(gb), birth_place(pt) {}
	~Player() {
		delete chess_box;
	}
public:
	//玩家得到地图上所有可以走的位置
	Points getAvailablePoints() const {
		Points res;
		if (chess_box->getChessNum() == ConstDefs::CHESSMAN_TOTAL) {
			res.push_back(birth_place);
		} else {
			res = mGameBoard->getAvailablePoints(player_id);
		}
		return res;
	}
	//玩家选择一颗棋子放置在棋盘上
	void putChess() const {
		Points pos = getAvailablePoints();
		size_t posl;
		Chessman::ShapeSet fs;
		for (ChessBox::ChessIter chess_iter = chess_box->begin(); chess_iter != chess_box->end(); ) {
			size_t plen = pos.size();
			size_t i;
			for (i = 0; i < plen; ++i) {
				fs = mGameBoard->getValidShapes(player_id, *chess_iter, pos[i].x, pos[i].y);
				if (!fs.empty()) {
					break;
				}
			}
			if (fs.empty()) {
				++chess_iter;
				continue;
			}
			mGameBoard->putChessAt(player_id, *chess_iter, fs.begin(), pos[i].x, pos[i].y);
			chess_box->removeChess(chess_iter);
			mGameBoard->showInScreen();
			printf("\n");
			break;
		}
	}
};