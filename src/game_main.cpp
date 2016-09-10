#include <iostream>
using std::cin; using std::cout; using std::endl;

#include "game_client.h"

#include "tester.h"

int main()
{
	//Tester::putAllChessAtMap(1);
	//Tester::playersAct(10);
	//Tester::registTeam();
	//Tester::testFlow(10);
	GameClient gc("127.0.0.1", 6000);
	gc.registTeam("god", 1001);
	gc.gameRun();
	cin.get();
}
