#pragma once

#include "team_info.h"
#include "socketman.h"
#include "message.h"
#include "game_board.h"
#include "player.h"

class GameClient {
public:
	SOCKET mClient;
	const TeamInfo *mTeam;
	GameBoard *mGameBoard;
	vector<Player*> mPlayers;
public:
	void registTeam(const string &tname, int tid) {
		mTeam = new TeamInfo(tname, tid);
		Socketman::sendMessage(MsgRegist(mTeam), mClient);
	}
	void gameStart() {
		const Message *msg = Socketman::recvMessage(mClient);
		const MsgGameStart *msg_st = dynamic_cast<const MsgGameStart*>(msg);
		if (msg_st == nullptr) {
		}
		else {
			mPlayers = msg_st->getPlayers();
		}
		for (Player *pl : mPlayers) {
			pl->mGameBoard = mGameBoard;
		}
		printf("game_start");
	}
	void gameRun() const {
		while (true) {
			const Message *msg = Socketman::recvMessage(mClient);
			const MsgInquire *iqr = dynamic_cast<const MsgInquire *>(msg);
			if (iqr == nullptr) {

			}
			else {

			}
		}
	}
public:
	GameClient(const string &ip, int port) {
		mClient = Socketman::createClientSocket(ip.c_str(), (u_short)port);
	}
	~GameClient() {
		Socketman::destroySocket(mClient);
	}
};
