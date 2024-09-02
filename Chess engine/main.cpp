#include <iostream>
#include <vector>
#include "piece/piece.h"

std::vector<std::vector<std::shared_ptr<piece>>> chessboard(8, std::vector<std::shared_ptr<piece>>(8, nullptr));
std::string player = "W";
bool endGame = false;

void placePiece(std::shared_ptr<piece> p, int row, int col) {
    chessboard[row][col] = p;
}

void printChessboard(const std::vector<std::vector<std::shared_ptr<piece>>>& chessboard) {
    int numRows = chessboard.size();
    int numCols = numRows > 0 ? chessboard[0].size() : 0;

    if (player == "W") std::cout << "White's move: " << std::endl;
    else std::cout << "Black's move" << std::endl;

    std::cout << "  ";
    for (int col = 0; col < numCols; ++col) {
        std::cout << col << " ";
    }
    std::cout << std::endl;

    for (int row = 0; row < numRows; ++row) {
        std::cout << row << " "; 
        for (const auto& cell : chessboard[row]) {
            if (cell) {
                std::cout << cell->name << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

void movePiece(int srcCol, int srcRow, int destCol, int destRow) {
    if (srcRow < 0 || srcRow >= chessboard.size() || srcCol < 0 || srcCol >= chessboard[0].size()) {
        std::cout << "Invalid move" << std::endl;
        return;
    }

    if (chessboard[srcRow][srcCol]) {
        std::shared_ptr<piece>& p = chessboard[srcRow][srcCol];
        std::string kingName = (isupper(p->name[0])) ? "K" : "k";
        std::string oponentKingName = (isupper(p->name[0])) ? "k" : "K";
        std::vector<std::vector<std::shared_ptr<piece>>> prev_chessboard(8, std::vector<std::shared_ptr<piece>>(8, nullptr));
        std::vector<std::vector<piece*>> rawChessboard(chessboard.size(), std::vector<piece*>(chessboard[0].size(), nullptr));
        for (size_t i = 0; i < chessboard.size(); ++i) {
            for (size_t j = 0; j < chessboard[i].size(); ++j) {
                rawChessboard[i][j] = chessboard[i][j].get();
                prev_chessboard[i][j] = chessboard[i][j];
            }
        }
        
        if (isupper(p->name[0]) && !isupper(player[0])) {
            std::cout << "Invalid move" << std::endl;
            return;
        }

        if (islower(p->name[0]) && !islower(player[0])) {
            std::cout << "Invalid move" << std::endl;
            return;
        }

        if (p->isValid(srcRow, srcCol, destRow, destCol, p->isKingSafe(rawChessboard, kingName), rawChessboard)) {
            std::shared_ptr<piece>& originalPiece = chessboard[destRow][destCol];
            chessboard[destRow][destCol] = p;
            p->hasMoved = true;

            if (p->madeEnpassantMove) {
                int direction = (p->name == "P") ? 1 : -1;
                chessboard[destRow - direction][destCol] = nullptr;
            }

            if (p->hasCastled) {
                int row = (isupper(p->name[0])) ? 0 : 7;
                if (p->castleDir == 1) {
                    chessboard[row][5] = chessboard[row][7];
                    chessboard[row][7] = nullptr;
                } else {
                    chessboard[row][3] = chessboard[row][0];
                    chessboard[row][3] = nullptr;
                }
            }

            chessboard[srcRow][srcCol] = nullptr;
            for (size_t i = 0; i < chessboard.size(); ++i) {
                for (size_t j = 0; j < chessboard[i].size(); ++j) {
                    rawChessboard[i][j] = chessboard[i][j].get();
                }
            }

            if (!p->isKingSafe(rawChessboard, kingName)) {
                for (size_t i = 0; i < prev_chessboard.size(); ++i) {
                    for (size_t j = 0; j < prev_chessboard[i].size(); ++j) {
                        chessboard[i][j] = prev_chessboard[i][j];
                    }
                }
                std::cout << "Invalid move" << std::endl;
            }

            if (p->Checkmate(rawChessboard, oponentKingName)) {
                endGame = true;
                return;
            }

            if (player == "W") player = "b";
            else player = "W";
        } else {
            std::cout << "Invalid move" << std::endl;
        }
    } else {
        std::cout << "Invalid move" << std::endl;
    }
}

int main() {
    auto whiteRook1 = std::make_shared<piece>("R", 1);
    auto whiteKnight1 = std::make_shared<piece>("N", 2);
    auto whiteBishop1 = std::make_shared<piece>("B", 3);
    auto whiteQueen = std::make_shared<piece>("Q", 4);
    auto whiteKing = std::make_shared<piece>("K", 5);
    auto whiteBishop2 = std::make_shared<piece>("B", 6);
    auto whiteKnight2 = std::make_shared<piece>("N", 7);
    auto whiteRook2 = std::make_shared<piece>("R", 8);

    placePiece(whiteRook1, 0, 0);
    placePiece(whiteKnight1, 0, 1);
    placePiece(whiteBishop1, 0, 2);
    placePiece(whiteQueen, 0, 3);
    placePiece(whiteKing, 0, 4);
    placePiece(whiteBishop2, 0, 5);
    placePiece(whiteKnight2, 0, 6);
    placePiece(whiteRook2, 0, 7);

    for (int col = 0; col < 8; ++col) {
        auto whitePawn = std::make_shared<piece>("P", 9 + col);
        placePiece(whitePawn, 1, col);
    }

    auto blackRook1 = std::make_shared<piece>("r", 17);
    auto blackKnight1 = std::make_shared<piece>("n", 18);
    auto blackBishop1 = std::make_shared<piece>("b", 19);
    auto blackQueen = std::make_shared<piece>("q", 20);
    auto blackKing = std::make_shared<piece>("k", 21);
    auto blackBishop2 = std::make_shared<piece>("b", 22);
    auto blackKnight2 = std::make_shared<piece>("n", 23);
    auto blackRook2 = std::make_shared<piece>("r", 24);

    placePiece(blackRook1, 7, 0);
    placePiece(blackKnight1, 7, 1);
    placePiece(blackBishop1, 7, 2);
    placePiece(blackQueen, 7, 3);
    placePiece(blackKing, 7, 4);
    placePiece(blackBishop2, 7, 5);
    placePiece(blackKnight2, 7, 6);
    placePiece(blackRook2, 7, 7);

    for (int col = 0; col < 8; ++col) {
        auto blackPawn = std::make_shared<piece>("p", 25 + col);
        placePiece(blackPawn, 6, col);
    }

    printChessboard(chessboard);

    int srcCol, srcRow, destCol, destRow;
    while (true) {
        std::cout << "Enter the source column (0-7): ";
        std::cin >> srcCol;
    
        std::cout << "Enter the source row (0-7): ";
        std::cin >> srcRow;

        std::cout << "Enter the destination column (0-7): ";
        std::cin >> destCol;

        std::cout << "Enter the destination row (0-7): ";
        std::cin >> destRow;

        movePiece(srcCol, srcRow, destCol, destRow);
        if (endGame) {
            if (player == "W") {
                std::cout << "White Wins" << std::endl;
            } else {
                std::cout << "Black Wins" << std::endl;
            }
            break;
        }  
        printChessboard(chessboard);
    }

    return 0;
}