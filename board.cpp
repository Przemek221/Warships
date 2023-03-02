//
// Przemyslaw Polej
//

#include "board.h"
#include "ship.h"

board::board() {
    for (auto &row: area) { //setting the board to the default state
        for (auto &col: row) {
            col = '*';
        }
    }
    for (int &i: shipCounter) { //resetting the ship counter
        i = 0;
    }

}

int board::print() { //prints the board
    int number = 1; //number
    int x = 0;

    //printing the axis (y)
    for (auto &row: area) {
        printf("%-2d|", number++);
        for (auto &col: row) {
            printf("%-3c", col);
        }
        putchar('\n');
    }

    //printing the axis (x)
    number = 65; // 65 is A in the ascii table
    printf("%-3c", ' ');
    for (int i = 0; i < 30; ++i) {
        if (i > 25) { //when we will reach the 'Z' letter
            number = 65;
            printf("%c%-2c", number, number + x); //displayed text will be: AA, AB, AC etc.
            x++;
        } else {
            printf("%-3c", number);
            number++;
        }
    }

    putchar('\n');

    return 0;
}

int board::searchForPosition(std::string *index, char numLine) {

    int i = 0;

    if (numLine == 'x') { //returned position depends on the axis
        while (i < 30 && *index != Xindex[i]) {
            i++;
        }

    } else {
        while (i < 30 && *index != Yindex[i]) {
            i++;
        }
    }

    return i;
}


int board::placeShip(ship *s) {
    int xDifference = (s->headPosition[0]) - (s->tailPosition[0]);
    int yDifference = (s->headPosition[1]) - (s->tailPosition[1]);
    char value = (s->tier + 48); //value of the ship size/tier (but in the char format)
    int x = s->headPosition[0];
    int y = s->headPosition[1];

// tab[rows][cols] or tab[y][x]

    if (xDifference == 0) {//if the x coordinates are the same (the ship is vertically oriented)

        if (yDifference > 0) { // if >0 then the head is under the tail
            for (int i = 0; i < s->tier; ++i) {
                area[y - i][x] = value;
            }
        } else {
            for (int i = 0; i < s->tier; ++i) {
                area[y + i][x] = value;
            }
        }

    } else {//if the y coordinates are the same (the ship is horizontally oriented)

        if (xDifference > 0) { //if >0 then the tail is closer to the beginning of the axis than the head
            //we're going to the tail
            for (int i = 0; i < s->tier; ++i) {
                area[y][x - i] = value;
            }
        } else {
            //we're also going to the tail
            for (int i = 0; i < s->tier; ++i) {
                area[y][x + i] = value;
            }
        }
    }

    return 1;
}

bool board::verifyPosition(ship *s) {
    int result = 0; //if =0 then sth is wrong, if =9 then it's ok
    int xDifference = (s->headPosition[0]) - (s->tailPosition[0]);
    int yDifference = (s->headPosition[1]) - (s->tailPosition[1]);

    ///verifying if the coordinates are not the same, if the size is okay or if they are horizontally or vertically oriented(only!!)

    if (xDifference != 0 && yDifference != 0) {//verifying vertical and horizontal orientation
        result = 0;
    } else {
        result++;//1
    }
    if (xDifference == 0 && yDifference == 0) {//if coordinates are not the same
        result = 0;
    } else {
        result++;//2
    }

    if (xDifference == 0) {//if x coordinates are the same
        (yDifference > 0) ? yDifference = yDifference + 1 : yDifference = (yDifference * (-1)) + 1; //size
        (yDifference == s->tier) ? result++ : result = 0;//if the size==tier
    } else {//if y coordinates are the same
        (xDifference > 0) ? xDifference = xDifference + 1 : xDifference = (xDifference * (-1)) + 1; //size
        (xDifference == s->tier) ? result++ : result = 0;//if the size==tier
    }

    //the result now should be 3

    ///if the ship coordinates are not in the board
    (s->tailPosition[0] > 29 || s->tailPosition[0] < 0) ? result = 0 : result++;
    (s->headPosition[0] > 29 || s->headPosition[0] < 0) ? result = 0 : result++;

    (s->tailPosition[1] > 29 || s->tailPosition[1] < 0) ? result = 0 : result++;
    (s->headPosition[1] > 29 || s->headPosition[1] < 0) ? result = 0 : result++;

    //the result now should be 7

    ///if the place is empty and the ship's side is not stick to the other ship's side

    // tab[rows][cols] or tab[y][x]
    int temp = s->tier, left = 0, right = 0; //the left and the right will be increased when the coordinate will be next to the other ship

    if (xDifference == 0) {//if the x coordinates are the same (the ship is vertically oriented)

        int x = s->headPosition[0];
        int y = s->headPosition[1];

        if ((s->headPosition[1]) - (s->tailPosition[1]) > 0) { //if >0 then the head is under the tail
            for (int i = 0; i < s->tier; ++i) {
                if (area[y - i][x] == '*') temp--;
                if (area[y - i][x + 1] == '1' || area[y - i][x + 1] == '2' || area[y - i][x + 1] == '3' ||
                    area[y - i][x + 1] == '4' || area[y - i][x + 1] == '5')
                    right++;
                if (area[y - i][x - 1] == '1' || area[y - i][x - 1] == '2' || area[y - i][x - 1] == '3' ||
                    area[y - i][x - 1] == '4' || area[y - i][x - 1] == '5')
                    left++;
            }
        } else {
            for (int i = 0; i < s->tier; ++i) {
                if (area[y + i][x] == '*') temp--;
                if (area[y + i][x + 1] == '1' || area[y + i][x + 1] == '2' || area[y + i][x + 1] == '3' ||
                    area[y + i][x + 1] == '4' || area[y + i][x + 1] == '5')
                    right++;
                if (area[y + i][x - 1] == '1' || area[y + i][x - 1] == '2' || area[y + i][x - 1] == '3' ||
                    area[y + i][x - 1] == '4' || area[y + i][x - 1] == '5')
                    left++;
            }
        }

    } else {//if the y coordinates are the same (the ship is horizontally oriented)

        int x = s->headPosition[0];
        int y = s->headPosition[1];

        if ((s->headPosition[0]) - (s->tailPosition[0]) > 0) { //if >0 then the tail is closer to the beginning of the axis than the head
            //we're going to the tail
            for (int i = 0; i < s->tier; ++i) {
                if (area[y][x - i] == '*') temp--;
                if (area[y + 1][x - i] == '1' || area[y + 1][x - i] == '2' || area[y + 1][x - i] == '3' ||
                    area[y + 1][x - i] == '4' || area[y + 1][x - i] == '5')
                    right++; //cell below
                if (area[y - 1][x - i] == '1' || area[y - 1][x - i] == '2' || area[y - 1][x - i] == '3' ||
                    area[y - 1][x - i] == '4' || area[y - 1][x - i] == '5')
                    left++; //cell above
            }
        } else {
            //we're also going to the tail
            for (int i = 0; i < s->tier; ++i) {
                if (area[y][x + i] == '*') temp--;
                if (area[y + 1][x + i] == '1' || area[y + 1][x + i] == '2' || area[y + 1][x + i] == '3' ||
                    area[y + 1][x + i] == '4' || area[y + 1][x + i] == '5')
                    right++; //cell below
                if (area[y - 1][x + i] == '1' || area[y - 1][x + i] == '2' || area[y - 1][x + i] == '3' ||
                    area[y - 1][x + i] == '4' || area[y - 1][x + i] == '5')
                    left++; //cell above
            }
        }
    }
    (temp == 0) ? result++ : result = 0;
    (right >= 2 || left >= 2) ? result = 0 : result++; //if sth is stick in more than one place

    //the result now should be 9

    return (result == 9);
}

char board::returnPosition(std::string &a, std::string &b) { //return coordinates [b][a]
    int x, y;
    x = searchForPosition(&a, 'x');
    y = searchForPosition(&b, 'y');
    return area[y][x];
}

int board::setPosition(int &x, int &y, char &val) { //sets the cell on the board as the "val"
    area[y][x] = val;
    return 0;
}

int board::hit(char &point, int &x, int &y) {

    //searching for the ship
    for (int i = 0; i < ships.size(); ++i) {

        if (ships[i].state[0] == point && !ships[i].state.empty()) { //if the index is the same as the point and the state of the ship is not empty

            if ((x >= ships[i].headPosition[0] && x <= ships[i].tailPosition[0]) ||
                    (x <= ships[i].headPosition[0] && x >= ships[i].tailPosition[0])) { //when the x of the point is in the ship's body

                if ((y >= ships[i].headPosition[1] && y <= ships[i].tailPosition[1]) ||
                    (y <= ships[i].headPosition[1] && y >= ships[i].tailPosition[1])) { //when the y of the point is in the ship's body

                    //now we know that the point is in the ship

                    ships[i].state.pop_back();

                    if (ships[i].state.empty()) { //if true then the ship is already destroyed

                        shipCounter[(point - '0')]--; // -1 on specific tier
                        ships.erase(ships.begin() + i); //deletes the ship from the list and also destructs the ship object

                        return 1;
                    } else {
                        return 0;
                    }

                }
            }
        }
    }

    return 0;
}



