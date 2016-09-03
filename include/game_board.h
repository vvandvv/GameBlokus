#pragma once

#include "const_defs.h"
#include "chessman.h"

class GameBoard {
	const int dirx[8] = { 1, -1,  0,  0,  1, -1,  1, -1 };
	const int diry[8] = { 0,  0,  1, -1, -1,  1,  1, -1 };
public:
	int mMap[ConstDefs::GAME_BOARD_WIDTH][ConstDefs::GAME_BOARD_WIDTH] = { 0 };
public:
	void putChessAt(int pid, const Chessman *chess, const Chessman::ShapeIter &shape, int x, int y) {
		for (const Point &pt : *shape) {
			mMap[pt.x + x][pt.y + y] = pid;
		}
	}
public:
	Chessman::ShapeSet getValidShapes(int pid, const Chessman *chess, int x, int y) const {
		Chessman::ShapeSet res;
		for (Chessman::ShapeIter fit = chess->begin(); fit != chess->end(); ++fit) {
			bool canput = true;
			for (const Point &pn : *fit) {
				if (!isValid(pid, pn.x + x, pn.y + y)) {
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
	bool isValid(int pid, int x, int y) const {
		bool res = false;
		bool inmap = isInMap(x, y) && mMap[x][y] == 0;
		if (inmap) {
			bool common = false;
			for (int i = 0; i < 4; ++i) {
				int tx = x + dirx[i], ty = y + diry[i];
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
	bool isHorn(int pid, int x, int y) const {
		bool res = false;
		bool inmap = isInMap(x, y);
		if (inmap) {
			bool horn = false;
			for (int i = 4; i < 8; ++i) {
				int tx = x + dirx[i], ty = y + diry[i];
				if (isInMap(tx, ty) && mMap[tx][ty] == pid) {
					horn = true;
					break;
				}
			}
			if (horn) {
				res = true;
			}
		}
		return res;
	}
	bool isInMap(int x, int y) const {
		return x >= 0 && x < ConstDefs::GAME_BOARD_WIDTH && y >= 0 && y < ConstDefs::GAME_BOARD_WIDTH;
	}
public:
	Points getAvailablePoints(int pid) const {
		Points res;
		for (size_t i = 0; i < ConstDefs::GAME_BOARD_WIDTH; ++i) {
			for (size_t j = 0; j < ConstDefs::GAME_BOARD_WIDTH; ++j) {
				if (isValid(pid, i, j) && isHorn(pid, i, j)) {
					res.push_back(Point(i, j));
				}
			}
		}
		return res;
	}
public:
	/**
	 * 显示棋盘到屏幕的（x, y）位置
	 */
	void showInScreen(int x = 0, int y = 0) const {
		HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(hdl, &info);
		COORD oxy = info.dwCursorPosition;
		SetConsoleTextAttribute(hdl, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		size_t i, j;
		for (size_t i = 0; i < ConstDefs::GAME_BOARD_WIDTH + 2; ++i) {
			for (size_t j = 0; j < ConstDefs::GAME_BOARD_WIDTH + 2; ++j) {
				printf("■");
			}
			printf("\n");
		}
		for (i = 0; i < ConstDefs::GAME_BOARD_WIDTH; ++i) {
			for (j = 0; j < ConstDefs::GAME_BOARD_WIDTH; ++j) {
				SetConsoleCursorPosition(hdl, { (SHORT)(oxy.X + (i + 1) * 2 + x), (SHORT)(oxy.Y + (ConstDefs::GAME_BOARD_WIDTH - j) + y) });
				SetConsoleTextAttribute(hdl, mMap[i][j] % 8 | FOREGROUND_INTENSITY);
				printf("■");
			}
		}
		SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		SetConsoleCursorPosition(hdl, { oxy.X, (SHORT)(oxy.Y + ConstDefs::GAME_BOARD_WIDTH + 2 + y) });
	}
	/**
	 * 在以（x, y）位置开始的棋盘上，显示一些点
	 */
	void showPosInScreen(const vector<Point> &pts, int x = 0, int y = 0) const {
		HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(hdl, &info);
		COORD oxy = info.dwCursorPosition;
		showInScreen(x, y);
		for (const Point &pt : pts) {
			SetConsoleCursorPosition(hdl, { (SHORT)(oxy.X + (pt.x + 1) * 2 + x), (SHORT)(oxy.Y + (ConstDefs::GAME_BOARD_WIDTH - pt.y) + y) });
			printf("  ");
		}
		SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		SetConsoleCursorPosition(hdl, { oxy.X, (SHORT)(oxy.Y + ConstDefs::GAME_BOARD_WIDTH + 2 + y) });
	}
};