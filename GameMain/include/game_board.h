#pragma once

#include "const_defs.h"
#include "chessman.h"

class GameBoard {
public:
	int mMap[ConstDefs::GAME_BOARD_WIDTH][ConstDefs::GAME_BOARD_WIDTH] = { 0 };
private:
	const int dirx[8] = { 1, -1,  0,  0,  1, -1,  1, -1 };
	const int diry[8] = { 0,  0,  1, -1, -1,  1,  1, -1 };
public:
	void putChessAt(int pid, const Chessman *chess, const Chessman::FormIter &form, int x, int y) {
		for (const Pane &pn : *form) {
			mMap[pn.x][pn.y] = pid;
		}
	}
public:
	Chessman::FormSet getValidForms(int pid, const Chessman *chess, int x, int y) const {
		Chessman::FormSet res;
		for (Chessman::FormIter fit = chess->begin(); fit != chess->end(); ++fit) {
			bool canput = true;
			for (const Pane &pn : *fit) {
				if (!isValid(pid, pn)) {
					canput = false;
					break;
				}
			}
			if (canput) {
				res.insert(*fit);
			}
		}
		return res;
	}
	bool isValid(int pid, const Pane &pn) const {
		bool res = false;
		bool inmap = isInMap(pn.x, pn.y);
		if (inmap) {
			bool common = false;
			for (int i = 0; i < 4; ++i) {
				int tx = pn.x + dirx[i], ty = pn.y + diry[i];
				if (isInMap(tx, ty) && mMap[tx][ty] == pid) {
					common = true;
					break;
				}
			}
			if (!common) {
				res = true;
			}
		}
		return res;
	}
	bool isInMap(int x, int y) const {
		return x >= 0 && x < ConstDefs::GAME_BOARD_WIDTH && y >= 0 && y < ConstDefs::GAME_BOARD_WIDTH;
	}
public:
	void showInScreen(int x = 0, int y = 0) const {
		HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(hdl, &info);
		COORD oxy = info.dwCursorPosition;
		SetConsoleTextAttribute(hdl, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		size_t i, j;
		for (size_t i = 0; i < ConstDefs::GAME_BOARD_WIDTH + 2; ++i) {
			for (size_t j = 0; j < ConstDefs::GAME_BOARD_WIDTH + 2; ++j) {
				printf("¡ö");
			}
			printf("\n");
		}
		for (i = 0; i < ConstDefs::GAME_BOARD_WIDTH; ++i) {
			for (j = 0; j < ConstDefs::GAME_BOARD_WIDTH; ++j) {
				SetConsoleCursorPosition(hdl, { (SHORT)(oxy.X + (i + 1) * 2 + x), (SHORT)(oxy.Y + (ConstDefs::GAME_BOARD_WIDTH - j) + y) });
				SetConsoleTextAttribute(hdl, mMap[i][j] % 8 | FOREGROUND_INTENSITY);
				printf("¡ö");
			}
		}
		SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		SetConsoleCursorPosition(hdl, { oxy.X, (SHORT)(oxy.Y + j + 2 + y) });
	}
};