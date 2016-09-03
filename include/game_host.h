#pragma once

#include <winsock2.h>

#include "json\json.h"

#include "const_defs.h"
#include "team_info.h"
#include "socketman.h"
#include "message.h"

class GameHost {
public:
	SOCKET mServer;
	SOCKET mSockets[ConstDefs::CONNECTION_POOL_SIZE] = { 0 };
	TeamInfo *mTeams[ConstDefs::CONNECTION_POOL_SIZE] = { nullptr };
private:
	int last_index = 0;
	Json::Reader reader;
	char msg_header_buf[ConstDefs::MSG_HEADER_LENGTH];
public:
	void acceptTeams() {
		Json::Value root;
		while (canAcceptClient()) {
			sockaddr adr; int adrlen = sizeof(adr);
			SOCKET acc = accept(mServer, &adr, &adrlen);
			if (acc == INVALID_SOCKET) {
				printf("accept() faild! (code:%d)\n", WSAGetLastError());
				closesocket(acc);
				continue;
			}
			recv(acc, msg_header_buf, sizeof(msg_header_buf), 0);
			int len = atoi(msg_header_buf);
			char *receiveMessage = (char*)malloc(len + 1);
			int ret = recv(acc, receiveMessage, len, 0);
			receiveMessage[len] = '\0';
			if (ret != len) {
				continue;
			}
			root.clear();
			reader.parse(receiveMessage, root, false);
			if (root["msg_name"].asString() == "registration") {
				mTeams[last_index] = TeamInfo::perseFromJson(root["msg_data"]);
				mSockets[last_index] = acc;
				++last_index;
				printf("receive message:%s\n", root.toStyledString().c_str());
			}
		}
	}
	void dropTeam(int index) {
		if (index >= 0 && index < ConstDefs::CONNECTION_POOL_SIZE) {
			closesocket(mSockets[index]);
			mSockets[index] = 0;
		}
	}
	bool canAcceptClient() const {
		return last_index < ConstDefs::CONNECTION_POOL_SIZE;
	}
	void startGame() const {
		for (SOCKET sk : mSockets) {
			Socketman::sendMessage(MsgGameStart(mTeams), sk);
		}
	}
	void runGame() const {
		//依次询问
		//收消息，超时 500ms
		//依次通知
	}
	void stopGame() const {
		//依次发出停止消息
	}
public:
	GameHost(const string &ip, int port) {
		mServer = Socketman::createServerSocket(ip.c_str(), (u_short)port);
	}
	~GameHost() {
		Socketman::destroySocket(mServer);
	}
};