#pragma once

#include "game_board.h"
#include "chessbox.h"

class Player {
public:
	int player_id;
	GameBoard *mGameBoard;
	ChessBox *mChessBox;
};