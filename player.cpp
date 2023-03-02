//
// Przemyslaw Polej
//

#include "player.h"

player::player(class board *board) : board(board) {
    nrOfShips = 0;
    this->enemyBoard = new class board;
}

player::player() = default;

player::~player() = default;