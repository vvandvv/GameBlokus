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
#include "point.h"

class Chessman {
	friend class ChessmanFactory;
public:
	class Cmp {
		static int _calcVal(const Shape &sps) {
			int sum = 0;
			for (const Point &pt : sps) {
				sum += pt.x + pt.y * ConstDefs::GAME_BOARD_WIDTH;
			}
			return sum;
		}
	public:
		bool operator()(const Shape &spl, const Shape &spr) const {
			return _calcVal(spl) < _calcVal(spr);
		}
	};
	typedef set<Shape, Cmp>::iterator ShapeIter;
	ShapeIter begin() const {
		return mShapes.begin();
	}
	ShapeIter end() const {
		return mShapes.end();
	}
	typedef set<Shape, Cmp> ShapeSet;
	ShapeSet mShapes;
public:
	int chess_id;
	Points points;
	string contact;
private:
	Chessman(int cid, const Points &pts, const string &ct) : chess_id(cid), points(pts), contact(ct) {
		Points cps = _getContactPoints();
		for (const Point &cp : cps) {
			_translation(cp.x, cp.y);
			for (size_t trf = 0; trf < 4; ++trf) {
				mShapes.insert(points);
				_flip_over();
				mShapes.insert(points);
				_flip_over();
				_rotate();
			}
			_translation(-cp.x, -cp.y);
		}
		//cout << mShapes.size() << endl;
	}
public: // 为了显示的一组函数
	/**
	 * 把棋子显示在一块 (ex, ey) 大小的棋盘上，接触点画在 (x, y) 指定的点上
	 */
	void showInScreen(bool isVertical = true, int x = 4, int y = 4, int ex = 9, int ey = 9) const {
		Chessman::showInScreen(points, isVertical, x, y, ex, ey);
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
		static Points cpn = _getContactPoints();
		if (cn >= 0 && cn < cpn.size()) {
			_translation(cpn[cn].x, cpn[cn].y);
		}
		return this;
	}
public:
	/**
	 * 把一些点显示在 （ex, ey） 的棋盘上，原点位置在 （x，y）点
	 */
	static void showInScreen(const Points &pns, bool isVertical = true, int x = 4, int y = 4, int ex = 9, int ey = 9) {
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
private: //变换需要的基本函数，对自身 points 做变换
	void _rotate() {
		int x, y;
		for (auto &pn : points) {
			x = pn.x;
			y = pn.y;
			pn.x = -y;
			pn.y = x;
		}
	}
	void _flip_over() {
		int x, y;
		for (auto &pn : points) {
			pn.x = -pn.x;
		}
	}
	void _translation(int x, int y) {
		for (auto &pn : points) {
			pn.x -= x;
			pn.y -= y;
		}
	}
	Shape _getContactPoints() const {
		static Shape res;
		if (res.size() == 0) {
			for (int ii = 0; ii < points.size(); ++ii) {
				if (contact[ii] == '1') {
					res.push_back(points[ii]);
				}
			}
		}
		return res;
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