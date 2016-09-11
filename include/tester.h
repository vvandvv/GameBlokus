#pragma once

#include "game_board.h"
#include "chessbox.h"

#include "player.h"
#include "team_info.h"
#include "message.h"

class Tester {
public:
	//显示所有的棋子，不同形状都会算作不同棋子
	static void showAllChess() {
		ChessBox cb;
		for (ChessBox::ChessIter chess_iter = cb.begin(); chess_iter != cb.end(); ++chess_iter) {
			const Chessman::ShapeSet &shapes = (*chess_iter)->mShapes;
			for (Chessman::ShapeIter shape_iter = shapes.begin(); shape_iter != shapes.end(); ++shape_iter) {
				(*chess_iter)->points = *shape_iter;
				(*chess_iter)->showInScreen();
				printf("\n");
			}
		}
	}
	//以某个点作为起始点，把所有的棋子放到棋盘上
	static void putAllChessAt(int pid, int x = 19, int y = 19) {
		GameBoard gb;
		gb.showInScreen();
		printf("\n");
		ChessBox cb;
		vector<Point> pos;
		pos.push_back({ x, y });
		Chessman::ShapeSet fs;
		for (ChessBox::ChessIter chess_iter = cb.begin(); chess_iter != cb.end(); ) {
			size_t plen = pos.size();
			size_t i;
			for (i = 0; i < plen; ++i) {
				fs = gb.getValidShapes(pid, *chess_iter, pos[i].x, pos[i].y);
				if (!fs.empty()) {
					break;
				}
			}
			if (fs.empty()) {
				++chess_iter;
				continue;
			}
			gb.putChessAt(pid, *chess_iter, fs.begin(), pos[i].x, pos[i].y);
			cb.removeChess(*chess_iter);
			chess_iter = cb.begin();
			gb.showInScreen();
			printf("\n");
			pos = gb.getAvailablePoints(pid);
			gb.showPosInScreen(pos);
			printf("%d\n", cb.getChessNum());
		}
	}
	//测试玩家的走 steps 步的样子
	static void playersAct(int thn) {
		GameBoard *gb = new GameBoard();
		Player pl1(1, gb, { 0, 0 });
		Player pl2(2, gb, { 0, 19 });
		Player pl3(3, gb, { 19, 19 });
		Player pl4(4, gb, { 19, 0 });
		for (int i = 0; i < thn; ++i) {
			pl1.putOneChess();
			Sleep(1000);
			pl2.putOneChess();
			Sleep(1000);
			pl3.putOneChess();
			Sleep(1000);
			pl4.putOneChess();
			Sleep(1000);
		}
		delete gb;
	}
	//用消息机制，测试玩家的走 steps 步的样子
	static void testFlow(int steps) {
		MsgGameStart *mgs = new MsgGameStart();
		vector<Player *> pls = mgs->getPlayers();
		GameBoard *gb = new GameBoard();
		int pid = 0;
		while (steps--) {
			for (const auto &pl : pls) {
				pl->mGameBoard = gb;
				Chessman *chess = pl->getNextChess();
				pl->putCurrentChess(chess);
				gb->showInScreen();
			}
		}
	}
};