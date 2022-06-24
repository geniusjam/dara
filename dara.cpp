#include <iostream>

#define ROWS 5
#define COLS 6

using namespace std;

short board[ROWS][COLS];

const char S[4] = " 12"; // SYMBOLS

short getMaxAdjacent(short r, short c, short p) {
    short x = r, y = c;

    short v = 1; // vertical
    x = r - 1;
    while (board[x][y] == p && x >= 0) {
        v++;
        x--;
    }
    x = r + 1;
    while (board[x][y] == p && x < ROWS) {
        v++;
        x++;
    }

    x = r;
    short h = 1; // horizontal
    y = c - 1;
    while (board[x][y] == p && y >= 0) {
        h++;
        y--;
    }
    y = c + 1;
    while (board[x][y] == p && y < COLS) {
        h++;
        y++;
    }

    return max(v, h);

    /*return (c + 2 < COLS && p == board[r][c + 1] && p == board[r][c + 2]) ||
        (c - 2 >= 0 && p == board[r][c - 1] && p == board[r][c - 2]) ||
        (c - 1 >= 0 && c + 1 < COLS && p == board[r][c-1] && p == board[r][c+1]) ||
        (r - 2 >= 0 && p == board[r - 1][c] && p == board[r - 2][c]) ||
        (r + 2 < ROWS && p == board[r + 1][c] && p == board[r + 2][c]) ||
        (r - 1 >= 0 && r + 1 < ROWS && p == board[r-1][c] && p == board[r+1][c]);*/
}

void printBoard() {
    for (int j = 0; j < COLS; j++) {
        cout << "+---+";
    }
    cout << "\n";
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << "| " << S[board[i][j]] << " |";
        }
        cout << "\n";
        for (int j = 0; j < COLS; j++) {
            cout << "+---+";
        }
        cout << "\n";
    }
}

void placeStone(bool p) {
    cout << "Player " << p + 1 << ": ";
    flush(cout);
    short r, c;
    cin >> r >> c;
    r--; c--;

    if (board[r][c] != 0) {
        cout << "That spot is taken! Please choose again.\n";
        placeStone(p);
        return;
    }

    // check for three-in-a-rows
    if ( getMaxAdjacent(r, c, p+1) >= 3 ) {
        cout << "You can't have 3 adjacent stones in the Drop Phase! Please choose again.\n";
        placeStone(p);
        return;
    }

    board[r][c] = p+1;

    cout << endl;
}

void makeMove(bool p) {
    cout << "Player " << p + 1 << ", make your move. First, select a stone to move. ";
    flush(cout);
    short r, c;
    cin >> r >> c;
    r--;c--;
    if (r < 0 || c < 0 || c >= COLS || r >= ROWS) {
        cout << "Invalid position! Please try again.\n";
        makeMove(p);
        return;
    }
    if (board[r][c] != p+1) {
        cout << "That's not your stone! Please try again.\n";
        makeMove(p);
        return;
    }

    cout << "Now, please choose a direction (u,d,r,l): ";
    char d;
    cin >> d;

    if (d != 'u' && d != 'd' && d != 'l' && d != 'r') {
        cout << "Invalid direction! Please try again.\n";
        makeMove(p);
        return;
    }
    
    short rn = r, cn = c; // new position
    if (d == 'u') rn--;
    else if (d == 'd') rn++;
    else if (d == 'r') cn++;
    else if (d == 'l') cn--;

    if (cn < 0 || rn < 0 || cn >= COLS || rn >= ROWS) {
        cout << "Invalid position! Please try again.\n";
        makeMove(p);
        return;
    }

    if (board[rn][cn] != 0) {
        cout << "That spot is taken! Please try again.\n";
        makeMove(p);
        return;
    }

    board[r][c] = 0;
    if (getMaxAdjacent(rn, cn, p+1) > 3) {
        cout << "It's illegal to have 4 or more adjacent stones! Please try a different move.\n";
        board[r][c] = p+1;
        makeMove(p);
        return;
    }

    board[rn][cn] = p+1;
    cout << "\n";

    if (getMaxAdjacent(rn, cn, p+1) == 3) {
        printBoard();
        cout << "\n\nYou made a three-in-a-row! ";
        while(1) {
            cout << "Pick a stone of your opponent to remove it: ";
            int x,y;
            cin >> x >> y;
            if (x < 1 || y < 1 || y > COLS || x > ROWS) {
                cout << "Invalid position! Please try again.\n";
                continue;
            }

            if (board[x-1][y-1] != (!p)+1) {
                cout << "That's not your opponent's stone! Please try again.\n";
                continue;
            }

            board[x-1][y-1] = 0;
            break;
        }

        short count = 0;
        for (short i = 0; i < ROWS; i++) {
            for (short j = 0; j < COLS; j++) {
                if (board[i][j] == (!p)+1) {
                    count++;
                    if (count > 2) {
                        return;
                    }
                }
            }
        }

        // count is < 3
        cout << "\n\n";
        printBoard();
        cout << "\n\n\n" << "Player " << p+1 << " has won! Congratulations!" << endl;
        exit(0);
    }
}

int main() {
    cout << "Welcome to the Dara game!" << endl;
    printBoard();
    cout << "You may now place your stones." << "\n\n";
    // Drop phase
    for (int i = 0; i < 5; i++) {
        placeStone(0);
        printBoard();
        cout << "\n\n";
        placeStone(1);
        printBoard();
        cout << "\n\n";
    }
    // Move phase

    do {
        makeMove(0);
        printBoard();
        cout << "\n\n";
        makeMove(1);
        printBoard();
        cout << "\n\n";
    } while(1);
}