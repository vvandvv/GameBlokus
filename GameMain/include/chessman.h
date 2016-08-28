#pragma once

#include <Windows.h>

#include <vector>
using std::vector;

#include <iostream>
using std::cout; using std::endl;

#include <string>
using std::string;

#include "pane.h"

class Chessman {
	friend class ChessmanFactory;
public:
	int chess_id;
	vector<Pane> mPanes;
	string contact;
private:
	Chessman(int cid, const vector<Pane> &pns, const string &ct) : chess_id(cid), mPanes(pns), contact(ct) {}
public:
	void showInScreen(int x = 0, int y = 0) const {
		HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(hdl, &info);
		COORD oxy = info.dwCursorPosition;
		SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);
		int index = 0;
		SHORT ymax = 0;
		for (auto pn : mPanes) {
			ymax = pn.y > ymax ? pn.y : ymax;
			SetConsoleCursorPosition(hdl, { (SHORT)(oxy.X + pn.x * 2 + x), (SHORT)(oxy.Y + pn.y + y) });
			if (contact[index] == '1') {
				SetConsoleTextAttribute(hdl, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			printf("¡ö");
			if (contact[index] == '1') {
				SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);
			}
			++index;
		}
		SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		SetConsoleCursorPosition(hdl, { oxy.X, (SHORT)(oxy.Y + ymax + 2 + y) });
	}
	Chessman *transform(int trf) {
		if (trf >= 0 && trf < 8) {
			if (trf >= 4) {
				_flip_over();
			}
			for (int ii = 0; ii < trf % 4; ++ii) {
				_rotate();
			}
		}
		return this;
	}
	Chessman *transformAt(int trf, int cn) {
		if (trf >= 0 && trf < 8) {
			if (trf >= 4) {
				_flip_over();
			}
			for (int ii = 0; ii < trf % 4; ++ii) {
				_rotate();
			}
		}
		vector<Pane> cpn = getContactPanes();
		if (cn >= 0 && cn < cpn.size()) {
			_translation(cpn[cn].x, cpn[cn].y);
		}
		return this;
	}
	vector<Pane> getContactPanes() const {
		vector<Pane> res;
		for (int ii = 0; ii < mPanes.size(); ++ii) {
			if (contact[ii] == '1') {
				res.push_back(mPanes[ii]);
			}
		}
		return res;
	}
private:
	void _rotate() {
		int x, y;
		for (auto &pn : mPanes) {
			x = pn.x;
			y = pn.y;
			pn.x = -y;
			pn.y = x;
		}
	}
	void _flip_over() {
		int x, y;
		for (auto &pn : mPanes) {
			pn.x = -pn.x;
		}
	}
	void _translation(int x, int y) {
		for (auto &pn : mPanes) {
			pn.x -= x;
			pn.y -= y;
		}
	}
};

class ChessmanFactory {
public:
	static Chessman *createChessman(int index) {
		switch (index) {
		case 0:
			return new Chessman(101, { {0, 0} }, "1");
		case 1:
			return new Chessman(201, { {0, 0},{0, 1} }, "11");
		case 2:
			return new Chessman(301, { { 0, 0 },{ 0, 1 },{ 0, 2 } }, "101");
		case 3:
			return new Chessman(302, { { 0, 0 },{ 0, 1 },{ 1, 0 } }, "111");
		case 4:
			return new Chessman(401, { { 0, 0 },{ 0, 1 },{ 0, 2 },{ 0, 3 } }, "1001");
		case 5:
			return new Chessman(402, { { 0, 0 },{ 0, 1 },{ 0, 2 },{ 1, 0 } }, "1011");
		case 6:
			return new Chessman(403, { { 0, 0 },{ 0, 1 },{ 0, 2 },{ 1, 1 } }, "1011");
		case 7:
			return new Chessman(404, { { 0, 0 },{ 0, 1 },{ 1, 0 },{ 1, 1 } }, "1111");
		case 8:
			return new Chessman(405, { { 0, 0 },{ 1, 0 },{ 1, 1 },{ 2, 1 } }, "1111");
		case 9:
			return new Chessman(501, { { 0, 0 },{ 0, 1 },{ 0, 2 },{ 0, 3 },{ 0, 4 } }, "10001");
		case 10:
			return new Chessman(502, { { 0, 0 },{ 0, 1 },{ 0, 2 },{ 0, 3 },{ 1, 0 } }, "10011");
		case 11:
			return new Chessman(503, { { 0, 0 },{ 0, 1 },{ 0, 2 },{ 0, 3 },{ 1, 1 } }, "10011");
		case 12:
			return new Chessman(504, { { 0, 0 },{ 0, 1 },{ 0, 2 },{ 1, 0 },{ 1, 1 } }, "10111");
		case 13:
			return new Chessman(505, { { 0, 0 },{ 0, 1 },{ 0, 2 },{ 1, 2 },{ 1, 3 } }, "10111");
		case 14:
			return new Chessman(506, { { 0, 0 },{ 0, 1 },{ 0, 2 },{ 1, 0 },{ 1, 2 } }, "10111");
		case 15:
			return new Chessman(507, { { 0, 0 },{ 1, 0 },{ 1, 1 },{ 1, 2 },{ 2, 0 } }, "10011");
		case 16:
			return new Chessman(508, { { 0, 0 },{ 0, 1 },{ 0, 2 },{ 1, 0 },{ 2, 0 } }, "10101");
		case 17:
			return new Chessman(509, { { 0, 0 },{ 0, 1 },{ 1, 1 },{ 1, 2 },{ 2, 2 } }, "11111");
		case 18:
			return new Chessman(510, { { 0, 0 },{ 0, 1 },{ 1, 1 },{ 2, 1 },{ 2, 2 } }, "11011");
		case 19:
			return new Chessman(511, { { 0, 0 },{ 0, 1 },{ 1, 1 },{ 1, 2 },{ 2, 1 } }, "11011");
		case 20:
			return new Chessman(512, { { 0, 1 },{ 1, 0 },{ 1, 1 },{ 1, 2 },{ 2, 1 } }, "11011");
		default:
			return nullptr;
		}
	}
};