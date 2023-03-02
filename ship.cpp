//
// Przemyslaw Polej
//

#include "ship.h"
#include <iostream>

ship::ship(int tier) : tier(tier) {
    for (int i = 0; i < tier; ++i) {
        this->state.push_back(tier + 48);
    }
}

ship::~ship() {
    tier = (-1);
    std::cout << "\n\nStatek zostal zatopiony\n\n";
}

ship::ship() = default;