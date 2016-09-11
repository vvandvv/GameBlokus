#pragma once

class ConstDefs {
public:
	enum { CONNECTION_POOL_SIZE = 2, REGISTER_MSG_LENGTH = 100, MSG_HEADER_LENGTH = 5, PLAYERS_NUM = 4, CHESS_POINTS_MAX = 5 };
	enum GameBoardConsts { GAME_BOARD_WIDTH = 20 };
	enum ChessBoxConsts { CHESSMAN_TOTAL = 21 };
	static char msg_header_buf[ConstDefs::MSG_HEADER_LENGTH];
};