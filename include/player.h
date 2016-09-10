#pragma once

#include "game_board.h"
#include "chessbox.h"
#include "message.h"

class Player {
	ChessBox *const chess_box = new ChessBox();
	const int player_id;
	const Point birth_place;
public:
	int getPlayerId() const {
		return player_id;
	}
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
	Chessman *getNextChess(const MsgInquire *iqr) const {
		Points pos = getAvailablePoints();
		Chessman::ShapeSet fs;
		ChessBox::ChessIter res;
		int ptx, pty;
		//遍历所有的棋子
		for (ChessBox::ChessIter chess_iter = chess_box->begin(); chess_iter != chess_box->end(); ) {
			//遍历所有可放置的位置
			for (const Point &ips : pos) {
				//得到在该位置所有可能的形状
				fs = mGameBoard->getValidShapes(player_id, *chess_iter, ips.x, ips.y);
				//如果找到可以放的形状，就不再寻找
				if (!fs.empty()) {
					ptx = ips.x, pty = ips.y;
					break;
				}
				//如果这个位置不能放该棋子，那么尝试下一个位置
			}
			//如果没有任何位置能放置该棋子，就继续下一颗棋子
			if (fs.empty()) {
				++chess_iter;
				continue;
			}
			//如果找到这颗棋子能够放的第一个位置，就取所有可放置形状的第一个
			(*chess_iter)->points = *fs.begin();
			(*chess_iter)->translatePoints(ptx, pty);
			return *chess_iter;
		}
		return nullptr;
	}
	void putCurrentChess(const Chessman *chess) {
		printf("player_id:%d, chess_id:%d\n", player_id, chess->chess_id);
		mGameBoard->putChess(player_id, chess);
		chess_box->removeChess(chess);
	}
};