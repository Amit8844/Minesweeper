#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

#define WHITE   "\033[37m"      /* White */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */
#define YELLOW  "\033[33m"      /* Yellow */
#define PURPLE  "\033[35m"      /* Purple */
#define ORANGE  "\033[33m"      /* Orange */
#define CYAN    "\033[36m"      /* Cyan */
#define GHOSTWHITE     "\033[37m"      /* GhostWhite */


bool GameEnds = false;
bool won = false;

void WelcomeScreen()
{
    cout << BLUE;
    cout << "                                           ____ ___   (_)____   ___   _____ _      __ ___   ___   ____   ___   _____" << endl;
    cout << "                                          / __ `__ \\ / // __ \\ / _ \\ / ___/| | /\\ / // _ \\ / _ \\ / __ \\ / _ \\ / ___/" << endl;
    cout << "                                         / / / / / // // / / //  __/(__  ) | |/ \\/ //  __//  __// /_/ //  __// /" << endl;
    cout << "                                        / / /_/ /_//_//_/ /_/ \\___/ \\___/  \\__/\\__/ \\___/ \\___// .___/ \\___//_/" << endl;
    cout << "                                       /_/                                                     /_/" << endl << endl;

    cout << WHITE << "\n\n\nRules:\n1. The board is divided into cells, with mines (" << RED << "*" << WHITE << ") randomly distributed.\n2. Board Size is 10 rows x 10 Columns and there are 10 mines.\n3. To win, you need to open all the cells which don't have mines.\n4. The number on a cell shows the number of mines adjacent to it. Using this information, you can determine cells that are safe, and cells that contain mines.\n\n\n";
    cout << GREEN;

    cout << WHITE << "\n\n";
};

void InitializeBoard(char board[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            board[i][j] = '?';
        }
    }
};

void PrintBoard(char board[10][10]) {
    for (int i = 0; i < 10; i++) {
        cout << i + 1 << "\t|";
        for (int j = 0; j < 10; j++) {
            if (board[i][j] == '*') {
                cout << RED << board[i][j] << WHITE << "|";
            }
            else if (board[i][j] == '1') {
                cout << BLUE << board[i][j] << WHITE << "|";
            }
            else if (board[i][j] == '2') {
                cout << GREEN << board[i][j] << WHITE << "|";
            }
            else if (board[i][j] == '3') {
                cout << CYAN << board[i][j] << WHITE << "|";
            }
            else if (board[i][j] == '4') {
                cout << PURPLE << board[i][j] << WHITE << "|";
            }
            else if (board[i][j] == '5') {
                cout << GHOSTWHITE << board[i][j] << WHITE << "|";
            }
            else if (board[i][j] == '?') {
                cout << YELLOW << board[i][j] << WHITE << "|";
            }
            else {
                cout << board[i][j] << "|";
            }
        }
        cout << endl;
    }
    cout << "\t ";
    for (int n = 1; n < 11; n++) {
        cout << n << " ";
    }
    cout << "\n\n\n";
}

void PrintBoard2(char board[10][10]) {
    for (int i = 0; i < 10; i++) {
        cout << i + 1 << "\t|";
        for (int j = 0; j < 10; j++) {
            if (board[i][j] == '*') {
                cout << RED << board[i][j] << WHITE << "|";
            }
            else {
                cout << board[i][j] << "|";
            }
        }
        cout << endl;
    }
    cout << "\t ";
    for (int n = 1; n < 11; n++) {
        cout << n << " ";
    }
    cout << "\n\n\n";
}

int CalculateNo(int x, int y, char board[10][10]) {
    int numBombs = 0;
    for (int x_offset = -1; x_offset <= 1; ++x_offset) {
        for (int y_offset = -1; y_offset <= 1; ++y_offset) {
            int nextRow = x + x_offset;
            int nextColumn = y + y_offset;
            if ((0 <= nextRow && nextRow < 10)
                && (0 <= nextColumn && nextColumn < 10)
                && board[nextRow][nextColumn] == '*') {
                numBombs++;
            }
        }
    }
    return numBombs;
}

void PlaceBombs(char board[10][10])
{
    vector<pair<int, int>> positions;
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            positions.push_back(make_pair(x, y));
        }
    }
    random_shuffle(positions.begin(), positions.end());
    for (int i = 0; i < 10; ++i)
    {
        board[positions[i].first][positions[i].second] = '*';
    }

    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            if (board[x][y] != '*') {
                board[x][y] = CalculateNo(x, y, board) + '0';
            }
        }
    }

}

void GameOver() {
    GameEnds = true;
    cout << RED << "\nGame Over!!!!!!!!   \nYou Stepped On The Mine! \nBetter luck Next Time.";
}

void GameWon() {
    GameEnds = true;
    cout << GREEN << "\nYou WON!!!!!! \nThank You for Playing \n";
}

void OpenAdjacent(int x, int y, char board[10][10], char showBoard[10][10]) {
    for (int x_offset = -1; x_offset <= 1; ++x_offset) {
        for (int y_offset = -1; y_offset <= 1; ++y_offset) {
            int nextRow = x + x_offset;
            int nextColumn = y + y_offset;

            if ((0 <= nextRow && nextRow < 10) && (0 <= nextColumn && nextColumn < 10)) {
                if (board[nextRow][nextColumn] == '0' && showBoard[nextRow][nextColumn] == '?') {
                    showBoard[nextRow][nextColumn] = board[nextRow][nextColumn];
                    OpenAdjacent(nextRow, nextColumn, board, showBoard);
                    continue;
                }
                showBoard[nextRow][nextColumn] = board[nextRow][nextColumn];
            }
        }
    }
}

void ProcessInput(int x, int y, char board[10][10], char showBoard[10][10])
{
    if (board[x][y] == '*')
    {
        showBoard[x][y] = '*';
        PrintBoard(showBoard);
        GameOver();
        return;
    }

    if (board[x][y] == '0') {
        OpenAdjacent(x, y, board, showBoard);
    }
    else {
        showBoard[x][y] = board[x][y];
    }

}

bool CheckIfWon(char board[10][10], char showBoard[10][10]) {
    bool won = true;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (showBoard[i][j] != board[i][j]) {
                won = false;
                break;
            }
        }
    }
    return won;
}

void SetMine(int x, int y, char showBoard[10][10]) {
    showBoard[x][y] = '*';
}


int main() {
    WelcomeScreen();
    char board[10][10];
    InitializeBoard(board);
    PlaceBombs(board);
    // PrintBoard(board);

    char showBoard[10][10];
    InitializeBoard(showBoard);
    // PrintBoard(showBoard);

    cout << YELLOW << "Start the Game?(Y/N): \t";
    char c;
    cin >> c;
    if (c == 'n' || c == 'N') {
        cout << "\nThank You for Playing\n";
        return 0;
    }


    while (!GameEnds) {

        cout << WHITE << "\033[2J\033[0;0H";

        PrintBoard(showBoard);

        cout << "Select an Option: \n" << YELLOW << "1. Open a Cell \n2. Set a mine\n" << WHITE << endl;
        int o;
        cin >> o;

        switch (o) {
        case 1:
            cout << WHITE << "Enter the row number which you want to select: \t";
            int r, c;
            cin >> r;
            cout << WHITE << "Enter the column number which you want to select: \t";
            cin >> c;
            ProcessInput(r - 1, c - 1, board, showBoard);
            if (CheckIfWon(board, showBoard)) {
                GameWon();
            }
            break;
        case 2:
            cout << WHITE << "Enter the row number which you want to select: \t";
            int r2, c2;
            cin >> r2;
            cout << WHITE << "Enter the column number which you want to select: \t";
            cin >> c2;
            SetMine(r2 - 1, c2 - 1, showBoard);
            break;
        default:
            cout << "Wrong Input";
            break;
        }
    }

}