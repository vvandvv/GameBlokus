#include "message.h"

#include "player.h"

Json::Value Message::toJsonObj() const {
	Json::Value jv;
	jv["msg_name"] = mMsgName;
	return jv;
}

vector<Player*> MsgGameStart::getPlayers() const {
	vector<Player*> res;
	for (int i = 0; i < ConstDefs::PLAYERS_NUM; ++i) {
		res[i] = new Player(i + 1, nullptr, birth_point[i]);
	}
	return res;
}