#pragma once

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

class Socketman {
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
	static void destroyServerSocket(SOCKET socket) {
		if (socket != INVALID_SOCKET) {
			closesocket(socket);
		}
		WSACleanup();
	}
private:
	~Socketman() {}
};