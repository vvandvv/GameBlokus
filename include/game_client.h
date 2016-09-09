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
	map<int, const Player *> mPlayers;
public:
	void registTeam(const string &tname, int tid) {
		mTeam = new TeamInfo(tname, tid);
		Socketman::sendMessage(MsgRegist(mTeam), mClient);
	}
	void gameStart() {
		const Message *msg = Socketman::recvMessage(mClient);
		
	}
	void gameRun() {
		while (true) {
			const Message *msg = Socketman::recvMessage(mClient);
			string msg_name = msg->mMsgName;
			if (msg_name == "game_start") {
				const MsgGameStart *msg_st = dynamic_cast<const MsgGameStart*>(msg);
				vector<Player *> pls;
				pls = msg_st->getPlayers();
				for (Player *pl : pls) {
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
