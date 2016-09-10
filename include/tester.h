#pragma once

#include "game_board.h"
#include "chessbox.h"

#include "player.h"
#include "team_info.h"
#include "message.h"

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
			cb.removeChess(chess_iter);
			chess_iter = cb.begin();
			gb.showInScreen();
			printf("\n");
			pos = gb.getAvailablePoints(pid);
			gb.showPosInScreen(pos);
			printf("%d\n", cb.getChessNum());
		}
	}
	static void playersAct(int thn) {
		GameBoard *gb = new GameBoard();
		Player pl1(1, gb, { 0, 0 });
		Player pl2(2, gb, { 0, 19 });
		Player pl3(3, gb, { 19, 19 });
		Player pl4(4, gb, { 19, 0 });
		for (int i = 0; i < thn; ++i) {
			pl1.putChess();
			_sleep(1000);
			pl2.putChess();
			_sleep(1000);
			pl3.putChess();
			_sleep(1000);
			pl4.putChess();
			_sleep(1000);
		}
		delete gb;
	}
	static void registTeam() {
		TeamInfo *ti = new TeamInfo("god", 1001);
		string msg = MsgRegist(ti).toJsonObj().toStyledString();
		printf("%s\n", msg.c_str());
	}
	static void testFlow(int steps) {
		MsgGameStart *mgs = new MsgGameStart();
		vector<Player *> pls = mgs->getPlayers();
		GameBoard *gb = new GameBoard();
		int pid = 0;
		while (steps--) {
			for (const auto &pl : pls) {
				pl->mGameBoard = gb;
				Chessman *chess = pl->getNextChess(new MsgInquire(0, 1001, pl->getPlayerId()));
				MsgAction *act = new MsgAction(RoundInfo(0, 0, ++pid % 4), chess);
				MsgNotification *nnt = new MsgNotification(act);
				MsgNotification *nntc = new MsgNotification(nnt->toJsonObj()["msg_data"]);
				pl->putCurrentChess(nntc->getChess());
				gb->showInScreen();
			}
		}
	}
};