#pragma once

#include <string>
using std::string;

#include "json\json.h"

#include "const_defs.h"
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
	MsgRegist(const TeamInfo *ti) : Message("registration"), team(ti) {}
	Json::Value toJsonObj() const {
		Json::Value jv = Message::toJsonObj();
		jv["msg_data"] = team->toJsonObj();
		return jv;
	}
};

/**
 * server msg
 */
class MsgGameStart : public Message {
	TeamInfo *const *info;
	Point birth_point[ConstDefs::PLAYERS_NUM] = { {0, 0}, {0, 19}, {19, 19}, {19, 0} };
public:
	MsgGameStart(TeamInfo *const infos[]) : Message("game_start"), info(infos) {}
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
};