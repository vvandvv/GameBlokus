#pragma once

#include "const_defs.h"
#include "chessman.h"

class ChessBox {
	int player_id;
public:
	typedef set<Chessman*>::iterator ChessIter;
	ChessIter begin() const {
		return mChesses.begin();
	}
	ChessIter end() const {
		return mChesses.end();
	}
	set<Chessman*> mChesses;
public:
	ChessBox() {
		for (size_t i = 0; i < ConstDefs::CHESSMAN_TOTAL; ++i) {
			mChesses.insert(ChessmanFactory::createChessman(i));
		}
	}
	~ChessBox() {
		for (ChessIter iter = mChesses.begin(); iter != mChesses.end(); ++iter) {
			delete *iter;
		}
	}
public:
	void removeChess(const ChessIter &iter) {
		delete *iter;
		mChesses.erase(iter);
	}
	size_t getChessNum() const {
		return mChesses.size();
	}
};
