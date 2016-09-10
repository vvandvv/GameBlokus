#pragma once

#include "json\json.h"

class RoundInfo {
	int hand_no;
public:
	int mTeamId;
	int mPlayerId;
public:
	RoundInfo(const Json::Value &jv) : hand_no(jv["hand_no"].asInt()), mTeamId(jv["team_id"].asInt()), mPlayerId(jv["player_id"].asInt()) {}
	RoundInfo(int hn, int ti, int pi) : hand_no(hn), mTeamId(ti), mPlayerId(pi) {}
	Json::Value toJsonObj() const {
		Json::Value jv;
		jv["hand_no"] = hand_no;
		jv["team_id"] = mTeamId;
		jv["player_id"] = mPlayerId;
		return jv;
	}
};