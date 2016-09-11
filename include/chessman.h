#pragma once

#include <Windows.h>

#include <string>
using std::string;
#include <set>
using std::set;
#include <bitset>
using std::bitset;

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
	//可选的所有形状
	ShapeSet mShapes;
public:
	int chess_id;
	//初始占据的点，也是最终选定的形状
	Points points;
	//可以作为接触点的位置
	bitset<ConstDefs::CHESS_POINTS_MAX> contact;
private:
	Chessman(int cid, const Points &pts, const string &ct = "") : chess_id(cid), points(pts), contact(ct) {
		size_t len = pts.size();
		for (size_t i = 0; i < len; ++i) {
			if (!contact.test(i)) {
				continue;
			}
			_translation(pts[i].x, pts[i].y);
			for (size_t trf = 0; trf < 4; ++trf) {
				mShapes.insert(points);
				_flip_over();
				mShapes.insert(points);
				_flip_over();
				_rotate();
			}
			_translation(-pts[i].x, -pts[i].y);
		}
	}
public:
	//把棋子的 points 显示在一块 (ex, ey) 大小的棋盘上，接触点画在 (x, y) 指定的点上
	void showInScreen(bool isVertical = true, size_t x = 4, size_t y = 4, size_t ex = 9, size_t ey = 9) {
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
		for (const auto &pn : points) {
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
	//对自身 points 做变换，逆时针旋转 90 度
	void _rotate() {
		int x, y;
		for (auto &pn : points) {
			x = pn.x;
			y = pn.y;
			pn.x = -y;
			pn.y = x;
		}
	}
	//对自身 points 做变换，左右翻转
	void _flip_over() {
		for (auto &pn : points) {
			pn.x = -pn.x;
		}
	}
	//对自身 points 做变换，以 （x，y）做原点
	void _translation(int x, int y) {
		for (auto &pn : points) {
			pn.x -= x;
			pn.y -= y;
		}
	}
public:
	//把 points 平移到（x，y）位置
	void translatePoints(int x, int y) {
		for (auto &pn : points) {
			pn.x += x;
			pn.y += y;
		}
	}
public:
	static Chessman *perseFromJson(const Json::Value &jv) {
		Points pts;
		Json::Value jva = jv["squareness"];
		for (const Json::Value &jvad : jva) {
			pts.push_back({ jvad["x"].asInt(), jvad["y"].asInt() });
		}
		return new Chessman(jv["id"].asInt(), pts);
	}
	Json::Value toJsonObj() const {
		Json::Value jv;
		jv["id"] = chess_id;
		Json::Value jva;
		Json::Value jvad;
		for (const Point &pt : points) {
			jvad["x"] = pt.x;
			jvad["y"] = pt.y;
			jva.append(jvad);
		}
		jv["squareness"] = jva;
		return jv;
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