#pragma once

#include <Windows.h>

#include <vector>
using std::vector;

#include <iostream>
using std::cout; using std::endl;

#include <string>
using std::string;

#include <set>
using std::set;

#include "const_defs.h"
#include "pane.h"

class Chessman {
	friend class ChessmanFactory;
public:
	class Cmp {
	public:
		bool operator()(const vector<Pane> &psl, const vector<Pane> &psr) const {
			return _calcVal(psl) < _calcVal(psr);
		}
	};
	typedef set<vector<Pane>, Cmp>::iterator FormIter;
	FormIter begin() const {
		return mForms.begin();
	}
	FormIter end() const {
		return mForms.end();
	}
	typedef set<vector<Pane>, Cmp> FormSet;
	FormSet mForms;
public:
	int chess_id;
private:
	vector<Pane> panes;
	string contact;
private:
	Chessman(int cid, const vector<Pane> &pns, const string &ct) : chess_id(cid), panes(pns), contact(ct) {
		vector<Pane> cpn = _getContactPanes();
		size_t consize = cpn.size();
		for (size_t cn = 0; cn < consize; ++cn) {
			_translation(cpn[cn].x, cpn[cn].y);
			for (size_t trf = 0; trf < 4; ++trf) {
				mForms.insert(panes);
				_flip_over();
				mForms.insert(panes);
				_flip_over();
				_rotate();
			}
			_translation(-cpn[cn].x, -cpn[cn].y);
		}
		//cout << mForms.size() << endl;
	}
public: // 为了显示的一组函数
	/**
	 * 把棋子显示在一块 (ex, ey) 大小的棋盘上，接触点画在 (x, y) 指定的点上
	 */
	void showInScreen(bool isVertical = true, int x = 4, int y = 4, int ex = 9, int ey = 9) const {
		Chessman::showInScreen(panes, isVertical, x, y, ex, ey);
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
	Chessman *transformAt(int trf, size_t cn) {
		if (trf >= 0 && trf < 8) {
			if (trf >= 4) {
				_flip_over();
			}
			for (int ii = 0; ii < trf % 4; ++ii) {
				_rotate();
			}
		}
		vector<Pane> cpn = _getContactPanes();
		if (cn >= 0 && cn < cpn.size()) {
			_translation(cpn[cn].x, cpn[cn].y);
		}
		return this;
	}
public:
	static void showInScreen(const vector<Pane> &pns, bool isVertical = true, int x = 4, int y = 4, int ex = 9, int ey = 9) {
		HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(hdl, &info);
		COORD oxy = info.dwCursorPosition;
		SetConsoleTextAttribute(hdl, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		for (size_t i = 0; i < ex; ++i) {
			for (size_t j = 0; j < ey; ++j) {
				printf("■");
			}
			printf("\n");
		}
		SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);
		int index = 0;
		for (auto pn : pns) {
			BOOL res = SetConsoleCursorPosition(hdl, { (SHORT)(oxy.X + (pn.x + x) * 2), (SHORT)(oxy.Y - pn.y + y) });
			//BOOL res = SetConsoleCursorPosition(hdl, { (SHORT)(oxy.X + (pn.x + x) * 2), (SHORT)(oxy.Y + pn.y + y) });
			if (!pn.x && !pn.y) {
				SetConsoleTextAttribute(hdl, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			if (res) {
				printf("■");
			}
			if (!pn.x && !pn.y) {
				SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);
			}
			++index;
		}
		SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		SetConsoleCursorPosition(hdl, { (SHORT)(oxy.X + (isVertical ? 0 : ex) * 2), (SHORT)(oxy.Y + (isVertical ? ey : 0)) });
	}
private:
	void _rotate() {
		int x, y;
		for (auto &pn : panes) {
			x = pn.x;
			y = pn.y;
			pn.x = -y;
			pn.y = x;
		}
	}
	void _flip_over() {
		int x, y;
		for (auto &pn : panes) {
			pn.x = -pn.x;
		}
	}
	void _translation(int x, int y) {
		for (auto &pn : panes) {
			pn.x -= x;
			pn.y -= y;
		}
	}
	vector<Pane> _getContactPanes() const {
		vector<Pane> res;
		if (res.size() == 0) {
			for (int ii = 0; ii < panes.size(); ++ii) {
				if (contact[ii] == '1') {
					res.push_back(panes[ii]);
				}
			}
		}
		return res;
	}
	static int _calcVal(const vector<Pane> &pns) {
		size_t len = pns.size();
		int sum = 0;
		for (size_t i = 0; i < len; ++i) {
			sum += pns[i].x + pns[i].y * ConstDefs::GAME_BOARD_WIDTH;
		}
		return sum;
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