#pragma once

#include <string>
using std::string;

class TeamInfo {
public:
	string mTeamName;
	int mTeamId;
public:
	static TeamInfo perseFromJson(const Json::Value &jv) {
		TeamInfo ti;
		ti.mTeamName = jv["team_name"].asString();
		ti.mTeamId = jv["team_id"].asInt();
		return ti;
	}
};