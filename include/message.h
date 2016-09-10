#pragma once

#include <string>
using std::string;

#include "json\json.h"

#include "const_defs.h"
#include "chessman.h"
#include "point.h"
#include "team_info.h"
#include "round_info.h"

class Message {
public:
	string mMsgName;
public:
	Message(const string &name) : mMsgName(name) {}
	virtual Json::Value toJsonObj() const = 0;
};

/**
 * client msg
 */
class MsgRegist : public Message {
	const TeamInfo *team;
public:
	MsgRegist(const Json::Value &jv) : Message("registration"), team(new TeamInfo(jv["team_name"].asString(), jv["team_id"].asInt())) {}
	MsgRegist(const TeamInfo *ti) : Message("registration"), team(ti) {}
	Json::Value toJsonObj() const {
		Json::Value jv = Message::toJsonObj();
		jv["msg_data"] = team->toJsonObj();
		return jv;
	}
	const TeamInfo *getTeamInfo() const {
		return team;
	}
};

/**
 * server msg
 */
class MsgInquire : public Message {
	RoundInfo round;
public:
	RoundInfo getRoundInfo() const {
		return round;
	}
public:
	MsgInquire(const Json::Value &jv) : Message("inquire"), round(jv) {}
	MsgInquire(int hn, int ti, int pi) : Message("inquire"), round(hn, ti, pi) {}
	Json::Value toJsonObj() const {
		Json::Value jv = Message::toJsonObj();
		jv["msg_data"] = round.toJsonObj();
		return jv;
	}
};

/**
 * client msg
 */
class MsgAction : public Message {
	RoundInfo round;
	const Chessman *chess;
public:
	MsgAction(const Json::Value &jv) : Message("action"), round(jv), chess(Chessman::perseFromJson(jv["chessman"])) {}
	MsgAction(const RoundInfo &info, const Chessman *chs) : Message("action"), round(info), chess(chs) {}
	Json::Value toJsonObj() const {
		Json::Value jv = Message::toJsonObj();
		Json::Value jvb = round.toJsonObj();
		jvb["chessman"] = chess->toJsonObj();
		jv["msg_data"] = jvb;
		return jv;
	}
	const Chessman *getChess() const {
		return chess;
	}
	RoundInfo getRoundInfo() const {
		return round;
	}
public:
	~MsgAction() {
		delete chess;
	}
};


class Player;
/**
 * server msg
 */
class MsgGameStart : public Message {
	const TeamInfo *const *info;
	Point birth_point[ConstDefs::PLAYERS_NUM] = { {0, 0}, {0, 19}, {19, 19}, {19, 0} };
public:
	MsgGameStart(const TeamInfo *const infos[] = { nullptr }) : Message("game_start"), info(infos) {}
	Json::Value toJsonObj() const {
		Json::Value jv = Message::toJsonObj();
		Json::Value ja;
		Json::Value jav;
		for (int i = 0; i < ConstDefs::PLAYERS_NUM; ++i) {
			jav["team_id"] = info[i % 2]->mTeamId;
			jav["player_id"] = i + 1;
			jav["birth_place"] = birth_point[i].toJsonObj();
			ja.append(jav);
		}
		jv["msg_data"] = ja;
		return jv;
	}
	vector<Player*> getPlayers() const;
};

class MsgNotification : public Message {
	const MsgAction *action;
public:
	MsgNotification(const Json::Value &jv) : Message("notification"), action(new MsgAction(jv)) {}
	MsgNotification(const MsgAction *act) : Message("notification"), action(act) {}
	Json::Value toJsonObj() const {
		Json::Value jv = Message::toJsonObj();
		jv["msg_data"] = action->toJsonObj()["msg_data"];
		return jv;
	}
public:
	const Chessman *getChess() const {
		return action->getChess();
	}
	RoundInfo getRoundInfo() const {
		return action->getRoundInfo();
	}
};

/**
 * common
 */
class MsgError : public Message {
	string reason;
public:
	MsgError() : Message("error") {}
	MsgError(const string &rs) : Message("error"), reason(rs) {}
	Json::Value toJsonObj() const {
		Json::Value jv = Message::toJsonObj();
		jv["msg_data"] = reason;
		return jv;
	}
};