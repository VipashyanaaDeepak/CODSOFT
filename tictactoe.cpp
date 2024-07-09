#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

vector<vector<char>> board(3, vector<char>(3, ' '));
char currentPlayerMarker = 'X';

void Initialize() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = ' ';
        }
    }
    currentPlayerMarker = 'X';
}

void DisplayBoard() {
    cout << "-------------" << endl;
    for (int i = 0; i < 3; ++i) {
        cout << "| ";
        for (int j = 0; j < 3; ++j) {
            cout << board[i][j] << " | ";
        }
        cout << endl << "-------------" << endl;
    }
}

bool PlaceMarker(int row, int col) {
    if (row < 0 || row >= 3 || col < 0 || col >= 3)
        return false;

    if (board[row][col] == ' ') {
        board[row][col] = currentPlayerMarker;
        return true;
    }

    return false;
}

bool CheckForWin() {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
            return true;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')
            return true;
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ')
        return true;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ')
        return true;

    return false;
}

bool IsBoardFull() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ')
                return false;
        }
    }
    return true;
}

void SwitchPlayers() {
    currentPlayerMarker = (currentPlayerMarker == 'X') ? 'O' : 'X';
}

bool PlayAgain() {
    char choice;
    cout << "Do you want to play again? (y/n): ";
    cin >> choice;
    return (choice == 'y' || choice == 'Y');
}

int main() {
    bool gameOver = false;

    while (!gameOver) {
        Initialize();

        int row, col;
        while (true) {
            DisplayBoard();
            cout << "Player " << currentPlayerMarker << ", enter your move (row and column): ";
            cin >> row >> col;

            if (row < 1 || row > 3 || col < 1 || col > 3) {
                cout << "Invalid move! Row and column values must be between 1 and 3." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            row--; col--;

            if (board[row][col] != ' ') {
                cout << "That spot is already taken. Try again." << endl;
                continue;
            }

            if (!PlaceMarker(row, col)) {
                cout << "Error placing marker. Please try again." << endl;
                continue;
            }

            if (CheckForWin()) {
                DisplayBoard();
                cout << "Player " << currentPlayerMarker << " Wins! Congratulations!" << endl;
                break;
            }

            if (IsBoardFull()) {
                DisplayBoard();
                cout << "It's a draw!" << endl;
                break;
            }

            SwitchPlayers();
        }

        gameOver = !PlayAgain();
    }

    cout << "Thanks for playing Tic Tac Toe!" << endl;

    return 0;
}
