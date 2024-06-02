#include <iostream>
#include <vector>
#include <limits> 

const int PLAYER_X = 1; 
const int PLAYER_O = -1; 

struct Move {
    int row, col;
};

void printBoard(const std::vector<std::vector<int>>& board) {
    for (const auto& row : board) {
        for (int cell : row) {
            if (cell == PLAYER_X) std::cout << "X ";
            else if (cell == PLAYER_O) std::cout << "O ";
            else std::cout << ". ";
        }
        std::cout << std::endl;
    }
}

bool isMovesLeft(const std::vector<std::vector<int>>& board) {
    for (const auto& row : board) {
        for (int cell : row) {
            if (cell == 0) return true;
        }
    }
    return false;
}

int evaluate(const std::vector<std::vector<int>>& board) {
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == PLAYER_X) return 10;
            else if (board[row][0] == PLAYER_O) return -10;
        }
    }

    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == PLAYER_X) return 10;
            else if (board[0][col] == PLAYER_O) return -10;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER_X) return 10;
        else if (board[0][0] == PLAYER_O) return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER_X) return 10;
        else if (board[0][2] == PLAYER_O) return -10;
    }

    return 0;
}

int minimax(std::vector<std::vector<int>>& board, int depth, bool isMax, int player, int bot) {
    int score = evaluate(board);

    if (score == 10) return score - depth;
    if (score == -10) return score + depth;
    if (!isMovesLeft(board)) return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = player;
                    best = std::max(best, minimax(board, depth + 1, !isMax, player, bot));
                    board[i][j] = 0;
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = bot;
                    best = std::min(best, minimax(board, depth + 1, !isMax, player, bot));
                    board[i][j] = 0;
                }
            }
        }
        return best;
    }
}

Move findBestMove(std::vector<std::vector<int>>& board, int bot, int player) {
    int bestVal = 1000;
    Move bestMove = {-1, -1};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                board[i][j] = bot;
                int moveVal = minimax(board, 0, true, player, bot);
                board[i][j] = 0;
                if (moveVal < bestVal) {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

int main() {
    std::vector<std::vector<int>> board = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    int player, bot;
    char choice;
    std::cout << "Выберите, кем вы будете играть (X/O): ";
    std::cin >> choice;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (choice != 'X' && choice != 'x' && choice != 'O' && choice != 'o') {
        std::cout << "Неверный выбор. Пожалуйста, введите X или O: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (choice == 'X' || choice == 'x') {
        player = PLAYER_X;
        bot = PLAYER_O;
    } else {
        player = PLAYER_O;
        bot = PLAYER_X;
    }

    int turn = PLAYER_X;
    while (isMovesLeft(board) && evaluate(board) == 0) {
        printBoard(board);
        if (turn == player) {
            int row, col;
            std::cout << "Введите ваш ход (строка и столбец от 1 до 3): ";
            std::cin >> row >> col;
            row--; 
            col--; 
            if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == 0) {
                board[row][col] = player;
                turn = bot;
            } else {
                std::cout << "Некорректный ход! Попробуйте снова.\n";
            }
        } else {
            Move bestMove = findBestMove(board, bot, player);
            board[bestMove.row][bestMove.col] = bot;
            turn = player;
        }
    }

    printBoard(board);
    int score = evaluate(board);
    if (score == 10) std::cout << "Вы выиграли!\n";
    else if (score == -10) std::cout << "Вы проиграли!\n";
    else std::cout << "Ничья!\n";

    return 0;
}
