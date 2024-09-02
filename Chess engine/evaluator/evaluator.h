#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <unordered_map>
#include <vector>
#include <utility>
#include "../piece/piece.h"

std::pair<double, double> evaluate(std::vector<std::vector<piece*>>& chessboard);

#endif