#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <cctype>
#include "../piece/piece.h"

std::unordered_map<char, double> pieceValues = {
    {'P', 1.0},
    {'N', 3.0},
    {'B', 3.0},
    {'R', 5.0},
    {'Q', 9.0},
    {'K', 100.0}
};

std::pair<double, double> evaluate(std::vector<std::vector<piece*>>& chessboard) {
    double whiteEval = 0;
    double blackEval = 0;

    for (int row = 0; row < chessboard.size(); ++row) {
        for (int col = 0; col < chessboard[0].size(); ++col) {
            if (chessboard[row][col]) {
                if (isupper(chessboard[row][col]->name[0])) whiteEval += pieceValues[chessboard[row][col]->name[0]];
                else {
                    char piece = std::toupper(chessboard[row][col]->name[0]);
                    blackEval += pieceValues[piece];
                }
            }
        }
    }

    return {whiteEval, blackEval};
}