#pragma once

#include "const_defs.h"
#include "chessman.h"

class ChessBox {
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
	int player_id;
public:
	ChessBox() {
		for (size_t i = 0; i < ConstDefs::CHESSMAN_TOTAL; ++i) {
			mChesses.insert(ChessmanFactory::createChessman(i));
		}
	}
};
