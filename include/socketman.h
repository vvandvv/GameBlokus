#pragma once

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include "message.h"

class Socketman {
	static char msg_header_buf[ConstDefs::MSG_HEADER_LENGTH];
	static Json::Reader reader;
	static Json::Value root;
public:
	static SOCKET createServerSocket(const char *ip, u_short port) {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			return INVALID_SOCKET;
		}
		//创建套接字
		SOCKET sServer;
		if ((sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
			WSACleanup();
			return INVALID_SOCKET;
		}
		sockaddr_in sdrin;
		sdrin.sin_family = AF_INET;
		sdrin.sin_port = htons(port);
		sdrin.sin_addr.s_addr = inet_addr(ip);
		if (bind(sServer, (LPSOCKADDR)&sdrin, sizeof(sdrin)) == SOCKET_ERROR || listen(sServer, 5) == SOCKET_ERROR) {
			printf("bind() or listen() faild! code:%d\n", WSAGetLastError());
			closesocket(sServer);
			WSACleanup();
			return INVALID_SOCKET;
		}
		printf("Waiting for client connecting...\n");
		return sServer;
	}
	static SOCKET createClientSocket(const char *ip, u_short port) {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			return INVALID_SOCKET;
		}
		//创建套接字
		SOCKET sClient;
		if ((sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
			WSACleanup();
			return INVALID_SOCKET;
		}
		sockaddr_in sdrin;
		sdrin.sin_family = AF_INET;
		sdrin.sin_port = htons(port);
		sdrin.sin_addr.s_addr = inet_addr(ip);
		int ret = connect(sClient, (sockaddr*)&sdrin, sizeof(sdrin));
		if (ret == SOCKET_ERROR) {
			printf("connect() failed!\n");
			closesocket(sClient);
			WSACleanup();
			return INVALID_SOCKET;
		}
		printf("Client is connected to server.\n");
		return sClient;
	}
	static void destroySocket(SOCKET socket) {
		if (socket != INVALID_SOCKET) {
			closesocket(socket);
		}
		WSACleanup();
	}
public:
	static void sendMessage(const Message &msg, SOCKET socket) {
		Json::Value jv = msg.toJsonObj();
		string json_msg = jv.toStyledString();
		int len = json_msg.length();
		char *buf = (char*)malloc(len + 1 + ConstDefs::MSG_HEADER_LENGTH);
		sprintf(buf, "%05d%s", len, json_msg.c_str());
		send(socket, buf, strlen(buf), 0);
		free(buf);
	}
	static const Message *recvMessage(SOCKET socket) {
		int msglen = recv(socket, msg_header_buf, sizeof(msg_header_buf), 0);
		if (msglen == -1) {
			return new MsgError("msg receive timeout.");
		}
		int len = atoi(msg_header_buf);
		char *receiveMessage = (char*)malloc(len + 1);
		int ret = recv(socket, receiveMessage, len, 0);
		receiveMessage[len] = '\0';
		if (ret != len) {
			return new MsgError("msg length not match.");
		}
		root.clear();
		reader.parse(receiveMessage, root, false);
		string msg_name = root["msg_name"].asString();
		if (msg_name == "registration") {
			return new MsgRegist(root["msg_data"]);
		}
		else if (msg_name == "action") {
			return new MsgAction(root["msg_data"]);
		}
		else {
			return new MsgError("unknown message.");
		}
	}
private:
	~Socketman() {}
};