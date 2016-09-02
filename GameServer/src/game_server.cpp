#include <iostream>
using std::cout; using std::endl; using std::cin;

#include "game_host.h"
#include "socketman.h"

int main() {
	cout << "hello" << endl;
	GameHost gh("127.0.0.1", 6000);
	gh.acceptTeams();
	gh.gameStart();
	gh.gameOver();
	return 0;
}