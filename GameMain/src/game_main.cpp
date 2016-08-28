#include <iostream>
using std::cin;

#include "chessman.h"

int main()
{
	for (int ik = 0; ik < 21; ++ik) {
		for (int ii = 0; ii < 8; ++ii) {
			ChessmanFactory::createChessman(ik)->transformAt(ii, 0)->showInScreen(10, 10);
		}
	}
	system("pause");
}