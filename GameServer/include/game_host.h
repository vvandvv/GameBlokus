#pragma once

#include <winsock2.h>

#include "json\json.h"

#include "team_info.h"
#include "socketman.h"

class GameHost {
	enum ConstDefs { CONNECTION_POOL_SIZE = 2, REGISTER_MSG_LENGTH = 100, MSG_HEADER_LENGTH = 5 };
public:
	SOCKET mServer;
	SOCKET mSockets[CONNECTION_POOL_SIZE] = { 0 };
	TeamInfo mTeams[CONNECTION_POOL_SIZE];
private:
	int last_index = 0;
	Json::Reader reader;
	char msg_header_buf[MSG_HEADER_LENGTH];
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
			if (ret == len) {
				root.clear();
				reader.parse(receiveMessage, root, false);
				mTeams[last_index] = TeamInfo::perseFromJson(root["msg_info"]);
				mSockets[last_index] = acc;
				++last_index;
				printf("receive message:%s\n", root.toStyledString().c_str());
			}
		}
	}
	void dropTeam(int index) {
		if (index >= 0 && index < CONNECTION_POOL_SIZE) {
			closesocket(mSockets[index]);
			mSockets[index] = 0;
		}
	}
	bool canAcceptClient() const {
		return last_index < CONNECTION_POOL_SIZE;
	}
	void gameStart() const {}
	void gameOver() const {}
public:
	GameHost(const string &ip, int port) {
		mServer = Socketman::createServerSocket(ip.c_str(), (u_short)port);
	}
	~GameHost() {
		Socketman::destroyServerSocket(mServer);
	}
};