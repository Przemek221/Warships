//
// Przemyslaw Polej
//

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "ship.h"
#include "board.h"
#include "player.h"

#define shipAmount (m2+m3+m4+m5) // m's are defined in board.h

int choose;
int xy[2];//contains recently hit ship
int hIR1 = 0, hIR2 = 0; //number of shots hitted one after another (hits in row)
int mIR1 = 0, mIR2 = 0; //number of shots missed one after another (misses in row)

board board1, board2;
player *players[2];

void initializePlayers() {
    players[0] = new player(&board1);
    players[1] = new player(&board2);
}

bool verifyTier(board *b, int &tier) {

    if ((tier == 2 && b->shipCounter[tier] == m2)) tier = 0;
    if ((tier == 3 && b->shipCounter[tier] == m3)) tier = 0;
    if ((tier == 4 && b->shipCounter[tier] == m4)) tier = 0;
    if ((tier == 5 && b->shipCounter[tier] == m5)) tier = 0;

    return tier >= 2; //if >=2 -> return true
}

int manual(player &player, int i) { //manual placing of the ship

    system("cls");
    char p=(&player==players[0])?'1':'2';
    std::cout << "\n\nPlansza gracza "<<p<<"\n";

    board *b = player.board;

    int tier;
    std::string pos1, pos2;
    std::string position[4];

    do {
        b->print();
        std::cout << "\nWpisz rozmiar statku (2-5): ";
        std::cin >> tier;

        if (tier > 5 || tier < 2) { //if the tier is correct
            std::cout << "\n\tZla wartosc!!";
            system("timeout /t 1");
            system("cls");
        }

        if (!verifyTier(b, tier)) { //verifies if the ship can be constructed
            std::cout << "\n\tJuz wyczerpales maksymalna liczbe statkow o podanym rozmiarze\n";
            system("timeout /t 1");
            system("cls");
        }

    } while (tier < 2 || tier > 5);

    ship *s = new ship(tier);


    do {
        std::cout << "Wpisz wspolrzedne aby ustawic dziob statku:"
                     "\nX:";
        std::cin >> position[0];
        std::cout << "Y:";
        std::cin >> position[1];

        std::cout << "Wpisz wspolrzedne aby ustawic rufe statku:"
                     "\nX:";
        std::cin >> position[2];
        std::cout << "Y:";
        std::cin >> position[3];

        s->position = position;
        std::string *ptr = s->position;

        //looking for head poisition
        s->headPosition[0] = b->searchForPosition(ptr, 'x');
        ptr++;
        s->headPosition[1] = b->searchForPosition(ptr, 'y');
        ptr++;

        //looking for tail poisition
        s->tailPosition[0] = b->searchForPosition(ptr, 'x');
        ptr++;
        s->tailPosition[1] = b->searchForPosition(ptr, 'y');

    } while (!b->verifyPosition(s)); //verifies if the ship can be placed in the specified position

    if (b->placeShip(s) == 1) { //if the ship is placed
        std::cout << "\npostawiono\n";
        player.nrOfShips++;
        b->ships.push_back(*s);
        b->shipCounter[s->tier]++;
    }
    return 0;
}

int automatic(player &player, int i, bool EvE) { //automatic placing of the ship

    system("cls");

    board *b = player.board;
    int tier;

    srand(time(nullptr));

    do { //random tier
        tier = rand() % (5 + 1 - 2) + 2;
    } while (!verifyTier(b, tier));

    ship *s = new ship(tier);

    srand(time(nullptr));

    do {

        srand(time(nullptr));

        //random position
        s->headPosition[0] = rand() % (29 + 1 - 1) + 1;
        s->headPosition[1] = rand() % (29 + 1 - 1) + 1;

        //random choose if the ship is vertically or horizontally oriented

        if (rand() % 2 == 0) {//x coordinates are the same
            s->tailPosition[0] = s->headPosition[0];

            (rand() % 2 == 0) ?
                s->tailPosition[1] = (s->headPosition[1] + s->tier - 1) :
                    s->tailPosition[1] = (s->headPosition[1] - s->tier - 1);

        } else {//y coordinates are the same
            s->tailPosition[1] = s->headPosition[1];

            (rand() % 2 == 0) ?
                s->tailPosition[0] = (s->headPosition[0] + s->tier - 1) :
                    s->tailPosition[0] = ( s->headPosition[0] - s->tier - 1);

        }

    } while (!b->verifyPosition(s)); //if the position of the ship is okay

    if (b->placeShip(s) == 1) { //if the ship is placed
        player.nrOfShips++;
        b->ships.push_back(*s);
        b->shipCounter[s->tier]++;
    }

    if (EvE) { //if it's the computer vs computer game, then print feedback what happened

        char p=(&player==players[0])?'1':'2';

        player.board->print();

        std::cout << "\nPlansza gracza "<<p<<":\n";
        std::cout<<"\nGracz postawil statek rozmiaru: "<<s->tier;
        std::cout<<"\nKtory ma swoj dziob wspolrzednych ("<<s->headPosition[0]<<","<<s->headPosition[1]<<")";

        system("timeout /t 5");
    }
    return 0;
}

int chooseFill(int var) { //choosing if the ships should be placed manually or automatically

    int x, i = 0;
    char p[2] = {'a', 'a'};

    if (var == 0) {// if computer vs computer

        while (i < shipAmount) { //placing the ships
            automatic(*players[0], i, true);
            automatic(*players[1], i, true);
            i++;
        }
        return 1;

    }

    for (int j = 1; j < (var + 1); ++j) { //players will choose how they will place the ships
        std::cout << "\nGracz " << j <<
                  "\nPowiedz, jak chcesz rozstawic statki:\n"
                  "\t1.Recznie\n"
                  "\t2.Automatycznie (trzeba poczekac)\n";
        std::cin >> x;

        switch (x) {
            case 1:
                p[j - 1] = 'm';
                break;
            case 2:
                p[j - 1] = 'a';
                break;
            default:
                std::cout << "Zly wybor, konczenie programu\n";
                system("timeout /t 2");
                exit(0);
        }

    }

    while (i < shipAmount) { //placing the ships
        (p[0] == 'a') ? automatic(*players[0], i, false) : manual(*players[0], i);

        (p[1] == 'a') ? automatic(*players[1], i, false) : manual(*players[1], i);

        i++;
    }

    return 0;
}

void intro() {
    std::cout << "\nTo jest gra statki."
                 "\nWybierz tryb gry:"
                 "\n\t1.PvP - gracz vs gracz"
                 "\n\t2.PvE - gracz vs komputer"
                 "\n\t3.EvE - komputer vs komputer (jesli dysponuje sie duzymi pokladami czasu)"
                 "\n";
    std::cin >> choose;

    switch (choose) {
        case 1:
            chooseFill(2);
            break;
        case 2:
            chooseFill(1);
            break;
        case 3:
            chooseFill(0);
            break;
        default:
            std::cout << "Zly wybor, konczenie programu\n";
            system("timeout /t 2");
            exit(0);
    }
}

void info(player &player) {

    int var;
    char p = (&player == players[0]) ? '1' : '2';

    do {
        system("cls");
        std::cout << "Tura gracza " << p;
        std::cout << "\nLiczba pozostalych statkow: " << player.nrOfShips;
        std::cout << "\nWybierz, co chcesz zrobic:"
                     "\n\t1.Wyswietl swoja plansze"
                     "\n\t3.Wyswietl plansze strzalow (**debug option)"
                     "\n\t2.Oddaj strzal\n";
        std::cin >> var;

        switch (var) {
            case 1:
                system("cls");
                player.board->print();
                std::cout << "\nAby wyjsc, wpisz 1: ";
                std::cin >> var;
                break;
            case 2:
                system("cls");
                break;
            case 3:
                system("cls");
                player.enemyBoard->print();
                std::cout << "\nAby wyjsc, wpisz 1: ";
                std::cin >> var;
                break;
            default:
                std::cout << "\nZla wartosc!!\n";
                system("timeout /t 1");
                break;
        }

    } while (var != 2);
}

void infoAuto(player &player) { //info in case of the automatic gameplay
    char p = (&player == players[0]) ? '1' : '2';
    system("cls");
    std::cout << "\nTura gracza " << p;
    std::cout << "\nLiczba pozostalych statkow: " << player.nrOfShips<<std::endl;
}

int hit(player &player, int &playerNr, int &x, int &y, char val, char &point) { //function of the ship hit

    class player *enemy;

    //setting the player and the enemy
    if (playerNr == 1) {
        enemy = players[1];
    } else {
        enemy = players[0];
    }

    if (enemy->board->hit(point, x, y) == 1) { // function returns 1 if the ship is destroyed
        enemy->nrOfShips--; //if the ship is destroyed
    }

    player.enemyBoard->setPosition(x, y, val); //placing the proper mark on the player's enemy (preview of the shots fired) board
    enemy->board->setPosition(x, y, val); //placing the proper mark on the enemy's board

    return 1;
}

int miss(player &player, int &playerNr, int &x, int &y, char val) { //function of the ship miss

    class player *enemy;

    //setting the player and the enemy
    if (playerNr == 1) {
        enemy = players[1];
    } else {
        enemy = players[0];
    }

    player.enemyBoard->setPosition(x, y, val); //placing the proper mark on the player's enemy (preview of the shots fired) board
    enemy->board->setPosition(x, y, val); //placing the proper mark on the enemy's board

    return 1;
}

char manShoot(player &player, int playerNr) { //manual shoot

    class player *enemy;

    //setting the player and the enemy
    if (playerNr == 1) {
        enemy = players[1];
    } else {
        enemy = players[0];
    }

    std::string position[2];
    int x, y;
    char point;

    system("cls");

    info(player);

    player.enemyBoard->print();

    std::cout << "Wpisz wspolrzedne aby oddac strzal:"
                 "\nX:";
    std::cin >> position[0];
    std::cout << "Y:";
    std::cin >> position[1];


    point = enemy->board->returnPosition(position[0], position[1]);
    x = enemy->board->searchForPosition(&position[0], 'x');
    y = enemy->board->searchForPosition(&position[1], 'y');

    if (point == '*' || point == 'o') { //if the hit point is not the ship
        miss(player, playerNr, x, y, 'o');
        return 'o';
    } else if (point == 'X') {//if we will hit the 'X'
        //nothing
    } else { //if we will hit the ship
        hit(player, playerNr, x, y, 'X', point);
        return 'X';
    }

// tab[rows][cols] or tab[y][x]
    return 'o';
}

int autoShoot(player &player, int playerNr, int last) { //auto shoot

    class player *enemy;
    int x, y;
    char point;

    //setting the player and the enemy
    if (playerNr == 1) {
        enemy = players[1];
    } else {
        enemy = players[0];
    }


    player.enemyBoard->print();
    infoAuto(player);
    srand(time(nullptr));

    /// the auto shoot has simple optimization
    //random x and y
    if (last>0 && (xy[0]==29 || xy[0]==0 || xy[1]==29 || xy[1]==0)){
        //setting both and then change if needed
        x =xy[0]+1;
        y = xy[1]+1;

        if (xy[0]==29){//random (up or down or in the same place)
            x=rand()%2==0?xy[0]:xy[0]-1;
        }
        if (xy[0]==0){
            x=rand()%2==0?xy[0]:xy[0]+1;
        }
        if (xy[1]==29){
            y=rand()%2==0?xy[1]:xy[1]-1;
        }
        if (xy[1]==0){
            y=rand()%2==0?xy[1]:xy[1]+1;
        }
    }else if (last == 1) { // then x is the same and y goes down
        x = xy[0];
        y = xy[1] + 1;
    } else if (last == 2) { // then x is the same and y goes up
        x = xy[0];
        y = xy[1] - 1;
    } else if (last == 3) { // then x goes left and y is the same
        x = xy[0] - 1;
        y = xy[1];
    } else if (last == 4) { // then x goes right and y is the same
        x = xy[0] + 1;
        y = xy[1];
    } else {
        x = rand() % (29 + 1 - 1) + 1;
        y = rand() % (29 + 1 - 1) + 1;
    }

    std::cout<<"\nStrzal w punkt("<<board1.Xindex[x]<<","<<board1.Yindex[y]<<"): ";


    point = enemy->board->area[y][x]; //point = element from the board

    if (point == '*' || point == 'o') { //if the hit point is not the ship

        miss(player, playerNr, x, y, 'o');
        std::cout<<"pudlo!\n";
        system("timeout /t 5");
        return 0;

    } else if (point == 'X') { //if we will hit the 'X'

        std::cout<<"pudlo!\n";
        system("timeout /t 5");
        return 0;

    } else { //if the hit point is the ship

        hit(player, playerNr, x, y, 'X', point);

        //setting last hit ship
        xy[0] = x;
        xy[1] = y;

        std::cout<<"trafiono!\n";
        system("timeout /t 5");


        return 1;
    }

// tab[rows][cols] or tab[y][x]
}

bool checkIfEnd() {
    if (players[1]->nrOfShips == 0 || players[0]->nrOfShips == 0) {
        return true;
    } else {
        return false;
    }
}

int game() {
    if (choose == 3) { //eve - computer vs computer

        //setting variables to the start state
        hIR1 = 0;
        hIR2 = 0;
        mIR1 = 0;
        mIR2 = 0;
        xy[0] = 0;
        xy[1] = 0;

        while (players[0]->nrOfShips > 0 && players[1]->nrOfShips > 0) { //while the players have at least one ship

            //[AUTO]
            //when we missed 4 times in a row then we are shooting randomly
            //once we hit the ship, we are shooting 4 cells around

            if (autoShoot(*players[0], 1, hIR1) != 0) {
                hIR1++;
            } else if (mIR1 == 4) {
                hIR1 = 0;
            } else {
                mIR1++;
            }

            if (checkIfEnd()) break; //checking if the enemy still has the ships

            if (autoShoot(*players[1], 2, hIR2) != 0) {
                hIR2++;
            } else if (mIR2 == 4) {
                hIR2 = 0;
            } else {
                mIR2++;
            }

            if (checkIfEnd()) break; //checking if the enemy still has the ships
        }

    } else if (choose == 2) { //pve - computer vs player

        //setting variables to the start state
        hIR1 = 0;
        mIR1 = 0;
        xy[0] = 0;
        xy[1] = 0;

        while (players[0]->nrOfShips > 0 && players[1]->nrOfShips > 0) { //while the players have at least one ship

            manShoot(*players[0], 1);

            if (checkIfEnd()) break; //checking if the enemy still has the ships

            if (autoShoot(*players[1], 2, hIR1) != 0) {
                hIR1++;
            } else if (mIR2 == 4) {
                hIR1 = 0;
            } else {
                mIR1++;
            }

            if (checkIfEnd()) break; //checking if the enemy still has the ships
        }

    } else { //pvp

        while (players[0]->nrOfShips > 0 && players[1]->nrOfShips > 0) { //while the players have at least one ship

            manShoot(*players[0], 1);
            if (checkIfEnd()) break; //checking if the enemy still has the ships
            manShoot(*players[1], 2);
            if (checkIfEnd()) break; //checking if the enemy still has the ships

        }

    }

    system("cls");

    //verifying the winner
    if (players[1]->nrOfShips == 0) std::cout << "\n\tGra skonczona, gracz 1 wygral.\n";
    if (players[0]->nrOfShips == 0) std::cout << "\n\tGra skonczona, gracz 2 wygral.\n";

    return 0;
}


int main() {

    initializePlayers();
    intro();

    system("cls");

    if (choose==3) {

        std::cout << "\n\nPlansza gracza 1:\n";
        players[0]->board->print();
        std::cout << "\n\nPlansza gracza 2:\n";
        players[1]->board->print();
        system("timeout /t 20");

    }

    system("cls");

    game();

    putchar('\n');
    system("pause"); //pauses the console after the end of the game

    return 0;
}