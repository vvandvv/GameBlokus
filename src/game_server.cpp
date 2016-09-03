#include <iostream>
using std::cout; using std::endl; using std::cin;

#include "game_host.h"
#include "socketman.h"

int main() {
	GameHost gh("127.0.0.1", 6000);
	gh.acceptTeams();
	gh.startGame();
	gh.runGame();
	gh.stopGame();
	cin.get();
	return 0;
}