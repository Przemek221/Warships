//
// Przemyslaw Polej
//

#ifndef STATKI_SHIP_H
#define STATKI_SHIP_H


#include <ostream>
#include <vector>
#include "board.h"

class ship {
public:
    int tier; //size of the ship
    int headPosition[2]; //coordinates of the ship's head [0]:x [1]:y
    int tailPosition[2]; //coordinates of the ship's tail [0]:x [1]:y
    std::string *position; //index of the head and the tail (but the letters)
    std::vector<char> state; //if it's null, then the ship is destroyed

    virtual ~ship();

    explicit ship(int tier);

    ship();

};

#endif //STATKI_SHIP_H