#include "message.h"

Json::Value Message::toJsonObj() const {
	Json::Value jv;
	jv["msg_name"] = mMsgName;
	return jv;
}