#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <vector>

class piece {
public:
    std::string name;
    int id;
    int type;
    int castleDir;
    bool isEnpassantSquare = false;
    bool madeEnpassantMove = false;
    bool hasCastled = false;
    bool hasMoved = false;

    piece(const std::string& name, int id);
    bool isValid(int srcRow, int srcCol, int destRow, int destCol, bool isKingSafe, std::vector<std::vector<piece*>>& chessboard);
    bool isKingSafe(std::vector<std::vector<piece*>>& chessboard, std::string kingName);
    bool Checkmate(std::vector<std::vector<piece*>>& chessboard, std::string kingName);
};

#endif