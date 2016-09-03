#pragma once

#include <vector>
using std::vector;

#include "json\json.h"

class Point {
public:
	int x, y;
public:
	Point(int ax, int ay) : x(ax), y(ay) {}
public:
	Json::Value toJsonObj() const {
		Json::Value jv;
		jv["x"] = x;
		jv["y"] = y;
		return jv;
	}
};

typedef vector<Point> Points;
typedef vector<Point> Shape;