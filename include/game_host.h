#pragma once

#include <winsock2.h>

#include "json\json.h"

#include "const_defs.h"
#include "team_info.h"
#include "socketman.h"
#include "message.h"
#include "game_board.h"

class GameHost {
public:
	SOCKET mServer;
	SOCKET mSockets[ConstDefs::CONNECTION_POOL_SIZE] = { 0 };
	const TeamInfo *mTeams[ConstDefs::CONNECTION_POOL_SIZE] = { nullptr };
	GameBoard *mGameBoard = new GameBoard();
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
			const Message *msg = Socketman::recvMessage(acc);
			const MsgRegist *msg_reg = dynamic_cast<const MsgRegist*>(msg);
			if (msg_reg == nullptr) {
				printf("%s\n", msg->toJsonObj().toStyledString().c_str());
				continue;
			}
			else {
				printf("%s\n", msg_reg->toJsonObj().toStyledString().c_str());
			}
			int time_out = 500;
			setsockopt(acc, SOL_SOCKET, SO_RCVTIMEO, (char *)&time_out, sizeof(int));
			mSockets[last_index] = acc;
			mTeams[last_index] = msg_reg->getTeamInfo();
			++last_index;
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
		static int hand_no = 0;
		while (true) {
			for (int i = 0; i < ConstDefs::PLAYERS_NUM; ++i) {
				int player_id = i + 1;
				int con_id = i % 2;
				Socketman::sendMessage(MsgInquire(hand_no++, mTeams[con_id]->mTeamId, player_id), mSockets[con_id]);
				const Message *msg = Socketman::recvMessage(mSockets[con_id]);
				const MsgAction *msg_act = dynamic_cast<const MsgAction*>(msg);
				if (msg_act == nullptr) {
					printf("%s\n", msg->toJsonObj().toStyledString().c_str());
					for (SOCKET sk : mSockets) {
						Socketman::sendMessage(MsgNotification(Json::Value()), sk);
					}
				} else {
					bool res = mGameBoard->putChess(i + 1, msg_act->getChess());
					if (res) {
						mGameBoard->showInScreen();
						for (SOCKET sk : mSockets) {
							Socketman::sendMessage(MsgNotification(msg_act->toJsonObj()), sk);
						}
					}
				}
			}
		}
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