#pragma once

class Point {
public:
	int x, y;
public:
	Point(int ax, int ay) : x(ax), y(ay) {}
};

class Pane {
public:
	int x, y;
	int player_id;
	int chess_id;
public:
	Pane(int ax = 0, int ay = 0) : x(ax), y(ay) {
		player_id = chess_id = 0;
	}
};