#pragma once

#include "team_info.h"
#include "socketman.h"
#include "message.h"

class GameClient {
public:
	SOCKET mClient;
	const TeamInfo *mTeam;
public:
	void registTeam(const string &tname, int tid) {
		mTeam = new TeamInfo(tname, tid);
		Socketman::sendMessage(MsgRegist(mTeam), mClient);
	}
public:
	GameClient(const string &ip, int port) {
		mClient = Socketman::createClientSocket(ip.c_str(), (u_short)port);
	}
	~GameClient() {
		Socketman::destroySocket(mClient);
	}
};
