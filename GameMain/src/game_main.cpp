#include <iostream>
using std::cin; using std::cout; using std::endl;

#include "tester.h"

#pragma comment(lib,"ws2_32.lib")

int main()
{
	//Tester::putAllChessAtMap(1);
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) {
		return 0;
	}
	//创建套接字
	SOCKET sClient;
	if ((sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		WSACleanup();
		return 0;
	}
	sockaddr_in sdrin;
	sdrin.sin_family = AF_INET;
	sdrin.sin_port = htons(6000);
	sdrin.sin_addr.s_addr = inet_addr("127.0.0.1");
	int ret = connect(sClient, (sockaddr*)&sdrin, sizeof(sdrin));
	if (ret == SOCKET_ERROR) {
		printf("connect() failed!\n");
		closesocket(sClient);
		WSACleanup();
		return 0;
	}
	char *msg = "{\"msg_name\":\"register\",\"msg_info\":{\"team_name\":\"%s\",\"team_id\":%d}}";
	char buffer[200] = { '\0' };
	sprintf(buffer, msg, "god", 1001);
	char msg_with_length[200];
	sprintf(msg_with_length, "%05d%s", strlen(buffer), buffer);
	ret = send(sClient, msg_with_length, sizeof(msg_with_length), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("send() failed!\n");
	}
	else {
		printf("client info has been sent!");
	}
	closesocket(sClient); //关闭套接字
	WSACleanup();
	Tester::playersAct(10);
	cin.get();
}
