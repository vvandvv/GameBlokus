#include "game_host.h"

int main(int argc, char *const argv[]) {
	if (argc < 3) {
		printf("GameServer.exe [ip] [port]");
	}
	GameHost gh(argv[1], atoi(argv[2]) /*"127.0.0.1", 6000*/);
	gh.acceptTeams();
	gh.startGame();
	gh.runGame();
	gh.stopGame();
	return 0;
}