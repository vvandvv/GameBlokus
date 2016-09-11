#include "game_client.h"

int main(int argc, char *const argv[])
{
	if (argc < 4) {
		printf("GameMain.exe [ip] [port] [team_id]");
	}
	GameClient gc(argv[1], atoi(argv[2]) /*"127.0.0.1", 6000*/);
	gc.registTeam("god", atoi(argv[3]) /*"god", 1001*/);
	gc.gameRun();
	return 0;
}
