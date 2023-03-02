//
// Przemyslaw Polej
//

#ifndef STATKI_BOARD_H
#define STATKI_BOARD_H

#include <string>
#include <vector>

//maksymalne ilosci statkow dla danych tierow (m - max,2 - tier 2)
#define m2 6
#define m3 5
#define m4 4
#define m5 3

class ship;

class board {
public:
    int shipCounter[6];
    std::vector <ship> ships; //contain ships which are on the board
    char area[30][30]; //the board

    //Y coordinates
    const std::string Yindex[30]= {"1","2","3","4","5","6","7","8","9","10","11"
            ,"12","13","14","15","16","17","18","19","20","21"
            ,"22","23","24","25","26","27","28","29","30"};

    //X coordinates
    const std::string Xindex[30]={"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K"
            , "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U"
            , "V", "W", "X", "Y", "Z", "AA", "AB", "AC", "AD"};

    board();
    int print();
    int searchForPosition(std::string *index, char numLine);
    int placeShip(ship *s);
    bool verifyPosition(ship *s);
    char returnPosition(std::string &a, std::string &b);
    int setPosition(int &x, int &y, char &val);
    int hit(char &point,int &x, int &y);

    friend class ship;
};


#endif //STATKI_BOARD_H
