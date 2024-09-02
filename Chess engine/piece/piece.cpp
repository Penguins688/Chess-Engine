#include <string>
#include <iostream>
#include <vector>
#include "piece.h"

piece::piece(const std::string& name, int id) : name(name), id(id) {}

bool piece::isValid(int srcRow, int srcCol, int destRow, int destCol, bool isKingSafe, std::vector<std::vector<piece*>>& chessboard) {

    if (destRow < 0 || destRow >= chessboard.size() || destCol < 0 || destCol >= chessboard[0].size()) return false;

    const piece* destinationPiece = chessboard[destRow][destCol];
    
    if ((isupper(name[0]) && destinationPiece && isupper(destinationPiece->name[0])) || (islower(name[0]) && destinationPiece && islower(destinationPiece->name[0]))) return false;

    int dstCol = abs(srcCol - destCol);
    int dstRow = abs(srcRow - destRow);

    if (name == "K" || name == "k") {
        if (dstRow <= 1 && dstCol <= 1) return true;

        if (isKingSafe && !this->hasCastled && dstCol == 2 && dstRow == 0) {
            if (destCol > srcCol) {
                if (chessboard[srcRow][7] && (chessboard[srcRow][7]->name == "R" || chessboard[srcRow][7]->name == "r") && !chessboard[srcRow][7]->hasMoved) {

                    for (int col = srcCol + 1; col < 7; ++col) {
                        if (chessboard[srcRow][col] != nullptr) return false;
                    }
                    this->hasCastled = true;
                    this->castleDir = 1;
                    return true;
                }
            }
            else if (destCol < srcCol) {
                if (chessboard[srcRow][0] && (chessboard[srcRow][0]->name == "R" || chessboard[srcRow][0]->name == "r") && !chessboard[srcRow][0]->hasMoved) {

                    for (int col = srcCol - 1; col > 0; --col) {
                        if (chessboard[srcRow][col] != nullptr) return false;
                    }
                    this->hasCastled = true;
                    this->castleDir = -1;
                    return true;
                }
            }
        }
        return false;
    } else if (name == "R" || name == "r") {
        if (dstRow == 0 || dstCol == 0) {
            if (dstRow == 0) { 
                int startCol = std::min(srcCol, destCol) + 1;
                int endCol = std::max(srcCol, destCol);
                for (int col = startCol; col < endCol; col++) {
                    if (chessboard[srcRow][col] != nullptr) return false;
                }
            } else if (dstCol == 0) { 
                int startRow = std::min(srcRow, destRow) + 1;
                int endRow = std::max(srcRow, destRow);
                for (int row = startRow; row < endRow; row++) {
                    if (chessboard[row][srcCol] != nullptr) return false;
                }
            }
            return true;
        }
    } else if (name == "B" || name == "b") {
        if (dstRow == dstCol) {
            int rowStep = (destRow > srcRow) ? 1 : -1;
            int colStep = (destCol > srcCol) ? 1 : -1;
            int r = srcRow + rowStep;
            int c = srcCol + colStep;
            while (r != destRow && c != destCol) {
                if (chessboard[r][c] != nullptr) return false;
                r += rowStep;
                c += colStep;
            }
            return true;
        }
    } else if (name == "N" || name == "n") {
        return (dstRow == 2 && dstCol == 1) || (dstCol == 2 && dstRow == 1);
    } else if (name == "Q" || name == "q") { 
        if ((dstRow == dstCol) || (dstRow == 0 || dstCol == 0)) {
            if (dstRow == dstCol) {
                int rowStep = (destRow > srcRow) ? 1 : -1;
                int colStep = (destCol > srcCol) ? 1 : -1;
                int r = srcRow + rowStep;
                int c = srcCol + colStep;
                while (r != destRow && c != destCol) {
                    if (chessboard[r][c] != nullptr) return false;
                    r += rowStep;
                    c += colStep;
                }
            } else { 
                if (dstRow == 0) {
                    int startCol = std::min(srcCol, destCol) + 1;
                    int endCol = std::max(srcCol, destCol);
                    for (int col = startCol; col < endCol; col++) {
                        if (chessboard[srcRow][col] != nullptr) return false;
                    }
                } else if (dstCol == 0) {
                    int startRow = std::min(srcRow, destRow) + 1;
                    int endRow = std::max(srcRow, destRow);
                    for (int row = startRow; row < endRow; row++) {
                        if (chessboard[row][srcCol] != nullptr) return false;
                    }
                }
            }
            return true;
        }
    } else if (name == "P" || name == "p") {
        dstRow = srcRow - destRow;
        dstCol = srcCol - destCol;
        
        int direction = (name == "P") ? 1 : -1; 
        int startRow = (name == "P") ? 1 : 6;
        int endRow = (name == "P") ? 7 : 0;
        bool valid = false;

        if (srcCol == destCol && destRow == srcRow + direction && chessboard[destRow][destCol] == nullptr) valid = true;
        else if (srcCol == destCol && destRow == srcRow + 2 * direction && srcRow == startRow && chessboard[destRow][destCol] == nullptr) {
            valid = true;
            isEnpassantSquare = true;
        }
        else if (abs(srcCol - destCol) == 1 && destRow == srcRow + direction && destinationPiece != nullptr) {
            valid = true;
        } else if (abs(srcCol - destCol) == 1 && destRow == srcRow + direction && destinationPiece == nullptr) {
            piece* potentialEnPassantPiece = chessboard[srcRow][destCol];
            if (potentialEnPassantPiece != nullptr && potentialEnPassantPiece->isEnpassantSquare) {
                chessboard[srcRow][srcCol]->madeEnpassantMove = true;
                return true;
            }
        }
        if (valid && destRow == endRow) {
            if (islower(name[0])) {
                std::cout << "Enter new piece q/r/b/n: " << std::endl;
                std::string newName;
                std::cin >> newName;
                if (newName != "q" && newName != "r" && newName != "b" && newName != "n") {
                    return false;
                } else {
                    chessboard[srcRow][srcCol]->name = newName;
                    return true;
                }
            } else {
                std::cout << "Enter new piece Q/R/B/N" << std::endl;
                std::string newName;
                std::cin >> newName;
                if (newName != "Q" && newName != "R" && newName != "B" && newName != "N") {
                    return false;
                } else {
                    chessboard[srcRow][srcCol]->name = newName;
                    return true;
                }
            }
        } else return valid;
    }
    return false;
}

bool piece::isKingSafe(std::vector<std::vector<piece*>>& chessboard, std::string kingName) {
    int numRows = chessboard.size();
    int numCols = numRows > 0 ? chessboard[0].size() : 0;
    int kingRow = -1;
    int kingCol = -1;

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (chessboard[row][col]) {
                if (chessboard[row][col]->name == kingName) {
                    kingRow = row;
                    kingCol = col;
                    break;
                }
            }
        }
        if (kingRow != -1) break; 
    }

    if (kingRow == -1 || kingCol == -1) {
        std::cerr << "Error: King not found on the board." << std::endl;
        return false;
    }

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            piece* currentPiece = chessboard[row][col];
            if (currentPiece) {
                if ((islower(chessboard[kingRow][kingCol]->name[0]) && isupper(currentPiece->name[0])) || (isupper(chessboard[kingRow][kingCol]->name[0]) && islower(currentPiece->name[0]))) {
                    if (currentPiece->isValid(row, col, kingRow, kingCol, true, chessboard)) return false; 
                }
            }
        }
    }
    return true;
}

bool piece::Checkmate(std::vector<std::vector<piece*>>& chessboard, std::string kingName) {
    int numRows = chessboard.size();
    int numCols = numRows > 0 ? chessboard[0].size() : 0;
    int kingRow = -1;
    int kingCol = -1;

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (chessboard[row][col]) {
                if (chessboard[row][col]->name == kingName) {
                    kingRow = row;
                    kingCol = col;
                    break;
                }
            }
        }
        if (kingRow != -1) break; 
    }

    if (kingRow == -1 || kingCol == -1) {
        std::cerr << "Error: King not found on the board." << std::endl;
        return false;
    }

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            piece* currentPiece = chessboard[row][col];
            if (currentPiece) {
                if ((isupper(kingName[0]) && isupper(currentPiece->name[0])) || 
                    (islower(kingName[0]) && islower(currentPiece->name[0]))) {

                    for (int destRow = 0; destRow < numRows; ++destRow) {
                        for (int destCol = 0; destCol < numCols; ++destCol) {
                            piece* targetPiece = chessboard[destRow][destCol];

                            if (currentPiece->isValid(row, col, destRow, destCol, true, chessboard)) {
                                chessboard[destRow][destCol] = currentPiece;
                                chessboard[row][col] = nullptr;

                                bool kingIsSafe = chessboard[kingRow][kingCol]->isKingSafe(chessboard, kingName);

                                chessboard[row][col] = currentPiece;
                                chessboard[destRow][destCol] = targetPiece;

                                if (kingIsSafe) return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}