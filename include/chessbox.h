#pragma once

#include "const_defs.h"
#include "chessman.h"

class ChessBox {
public:
	class ChessCmp {
	public:
		bool operator()(const Chessman *chl, const Chessman *chr) {
			return chl->chess_id < chr->chess_id;
		}
	};
	typedef set<Chessman*, ChessCmp>::iterator ChessIter;
	ChessIter begin() const {
		return mChesses.begin();
	}
	ChessIter end() const {
		return mChesses.end();
	}
	set<Chessman *, ChessCmp> mChesses;
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
	void removeChess(const Chessman *chess) {
		ChessIter iter = mChesses.find(const_cast<Chessman *>(chess));
		printf("finding chess...\n");
		if (iter != mChesses.end()) {
			printf("chess found.\n");
			delete *iter;
			mChesses.erase(iter);
		}
	}
	size_t getChessNum() const {
		return mChesses.size();
	}
};
