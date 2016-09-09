#include "socketman.h"

char Socketman::msg_header_buf[ConstDefs::MSG_HEADER_LENGTH];
Json::Reader Socketman::reader;
Json::Value Socketman::root;