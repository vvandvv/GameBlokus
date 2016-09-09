#pragma once

#include <string>
using std::string;

#include "json\json.h"

#include "const_defs.h"
#include "chessman.h"
#include "point.h"
#include "team_info.h"

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
	int hand_no;
	int team_id;
	int player_id;
	friend class MsgAction;
public:
	MsgInquire(const Json::Value &jv) : Message("inquire"), hand_no(jv["hand_no"].asInt()), team_id(jv["team_id"].asInt()), player_id(jv["player_id"].asInt()) {}
	MsgInquire(int hn, int ti, int pi) : Message("inquire"), hand_no(hn), team_id(ti), player_id(pi) {}
	Json::Value toJsonObj() const {
		Json::Value jv = Message::toJsonObj();
		Json::Value jvb;
		jvb["hand_no"] = hand_no;
		jvb["team_id"] = team_id;
		jvb["player_id"] = player_id;
		jv["msg_data"] = jvb;
		return jv;
	}
};

/**
 * client msg
 */
class MsgAction : public Message {
	int hand_no;
	int team_id;
	int player_id;
	const Chessman *chess;
public:
	MsgAction(const Json::Value &jv) : Message("action"), hand_no(jv["hand_no"].asInt()), team_id(jv["team_id"].asInt()), player_id(jv["player_id"].asInt()), chess(Chessman::perseFromJson(jv["chessman"])) {}
	MsgAction(const MsgInquire *miqr, const Chessman *chs) : Message("action"), hand_no(miqr->hand_no), team_id(miqr->team_id), player_id(miqr->player_id), chess(chs) {}
	Json::Value toJsonObj() const {
		Json::Value jv = Message::toJsonObj();
		Json::Value jvb;
		jvb["hand_no"] = hand_no;
		jvb["team_id"] = team_id;
		jvb["player_id"] = player_id;
		jvb["chessman"] = chess->toJsonObj();
		jv["msg_data"] = jvb;
		return jv;
	}
	const Chessman *getChess() const {
		return chess;
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
	MsgGameStart(const TeamInfo *const infos[]) : Message("game_start"), info(infos) {}
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
		jv["msg_date"] = ja;
		return jv;
	}
	vector<Player*> getPlayers() const;
};

class MsgNotification : public Message {
	Json::Value action;
public:
	MsgNotification(const Json::Value &actJson) : Message("notification"), action(actJson) {}
	Json::Value toJsonObj() const {
		Json::Value jv = Message::toJsonObj();
		jv["msg_data"] = action["msg_data"];
		return jv;
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