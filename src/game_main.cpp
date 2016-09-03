#include <iostream>
using std::cin; using std::cout; using std::endl;

#include "game_client.h"

int main()
{
	//Tester::putAllChessAtMap(1);
	//Tester::playersAct(10);
	//Tester::registTeam();
	GameClient gc("127.0.0.1", 6000);
	gc.registTeam("god", 1001);
	cin.get();
}
