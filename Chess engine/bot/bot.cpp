#include <vector>
#include <iostream>
#include <utility>
#include "../evaluator/evaluator.h"
#include "../piece/piece.h"

std::pair<std::pair<int, int>, std::pair<int, int>> move(std::vector<std::vector<piece*>>& chessboard, std::string player) {

    std::vector<std::vector<std::vector<piece*>>> games;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> moves;
    std::vector<std::pair<double, double>> evaluations;

    for (int row = 0; row < chessboard.size(); ++row) {
        for (int col = 0; col < chessboard[0].size(); ++col) {
            if (chessboard[row][col]) {
                if ((isupper(player[0]) && isupper(chessboard[row][col]->name[0])) || (islower(player[0]) && islower(chessboard[row][col]->name[0]))) {
                    for (int row2 = 0; row2 < chessboard.size(); ++row2) {
                        for (int col2 = 0; col2 < chessboard[0].size(); ++col2) {
                            std::string kingName = isupper(player[0]) ? "K" : "k";
                            bool kingSafe = chessboard[row][col]->isKingSafe(chessboard, kingName);
                            if (chessboard[row][col]->isValid(row, col, row2, col2, kingSafe, chessboard)) {
                                std::vector<std::vector<piece*>> temporaryChessboard(chessboard.size(), std::vector<piece*>(chessboard[0].size(), nullptr));
                                for (size_t i = 0; i < chessboard.size(); ++i) {
                                    for (size_t j = 0; j < chessboard[i].size(); ++j) {
                                        temporaryChessboard[i][j] = chessboard[i][j];
                                    }
                                }
                                temporaryChessboard[row2][col2] = temporaryChessboard[row][col];
                                temporaryChessboard[row][col] = nullptr;
                                games.push_back(temporaryChessboard);
                                moves.push_back({{row, col}, {row2, col2}});
                            }
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < games.size(); ++i) evaluations.push_back(evaluate(games[i]));

    double bestEval = (player == "W") ? -10000 : 10000;
    int bestEvalIndex = -1;

    for (int i = 0; i < evaluations.size(); ++i) {
        double eval = evaluations[i].first - evaluations[i].second;
        if (player == "W") { 
            if (eval > bestEval) {
                bestEval = eval;
                bestEvalIndex = i;
            }
        } else {
            if (eval < bestEval) {
                bestEval = eval;
                bestEvalIndex = i;
            }
        }
    }

    return {moves[bestEvalIndex]};
}