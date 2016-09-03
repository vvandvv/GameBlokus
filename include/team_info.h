#pragma once

#include <string>
using std::string;

#include "json\json.h"

class TeamInfo {
public:
	string mTeamName;
	int mTeamId;
public:
	TeamInfo(const string &name, int id) : mTeamName(name), mTeamId(id) {}
public:
	static TeamInfo* perseFromJson(const Json::Value &jv) {
		return new TeamInfo(jv["team_name"].asString(), jv["team_id"].asInt());
	}
	Json::Value toJsonObj() const {
		Json::Value jv;
		jv["team_id"] = mTeamId;
		jv["team_name"] = mTeamName;
		return jv;
	}
};