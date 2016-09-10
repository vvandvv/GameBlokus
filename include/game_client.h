#pragma once

#include <map>
using std::map; using std::make_pair;

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
	map<int, Player *> mPlayers;
public:
	void registTeam(const string &tname, int tid) {
		mTeam = new TeamInfo(tname, tid);
		Socketman::sendMessage(MsgRegist(mTeam), mClient);
	}
	void gameRun() {
		while (true) {
			const Message *msg = Socketman::recvMessage(mClient);
			string msg_name = msg->mMsgName;
			if (msg_name == "game_start") {
				const MsgGameStart *msg_st = dynamic_cast<const MsgGameStart*>(msg);
				mGameBoard = new GameBoard();
				vector<Player *> pls;
				pls = msg_st->getPlayers();
				for (Player *pl : pls) {
					pl->mGameBoard = mGameBoard;
					mPlayers.insert(std::make_pair(pl->getPlayerId(), pl));
				}
			}
			else if (msg_name == "inquire") {
				const MsgInquire *iqr = dynamic_cast<const MsgInquire *>(msg);
				RoundInfo rinfo = iqr->getRoundInfo();
				Chessman *chess = mPlayers[rinfo.mPlayerId]->getNextChess(iqr);
				Socketman::sendMessage(MsgAction(rinfo, chess), mClient);
			}
			else if (msg_name == "notification") {
				const MsgNotification *nti = dynamic_cast<const MsgNotification *>(msg);
				RoundInfo rinfo = nti->getRoundInfo();
				mPlayers[rinfo.mPlayerId]->putCurrentChess(nti->getChess());
			} 
			else {
				//game_over
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
