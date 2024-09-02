#ifndef BOT_H
#define BOT_H

#include <vector>
#include <utility>
#include "../evaluator/evaluator.h"
#include "../piece/piece.h"

std::pair<std::pair<int, int>, std::pair<int, int>> move(std::vector<std::vector<piece*>>& chessboard, std::string player);

#endif