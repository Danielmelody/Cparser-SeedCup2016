//
// Created by 胡一鸣 on 16/10/27.
//

#ifndef C_PARSER_UTILITY_H
#define C_PARSER_UTILITY_H

#include <vector>
#include "Token.h"

using namespace std;

namespace cParser {
namespace Utility {
  vector<Token*> combineElseIf(vector<Token*>& tokens);
}
}

#endif //C_PARSER_UTILITY_H