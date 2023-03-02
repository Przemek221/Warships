//
// Przemyslaw Polej
//

#ifndef STATKI_PLAYER_H
#define STATKI_PLAYER_H

#include "board.h"

class board;

class player {
public:

    board *board;

    class board *enemyBoard;

    int nrOfShips;

    explicit player(class board *board);

    player();

    virtual ~player();

};

#endif //STATKI_PLAYER_H
