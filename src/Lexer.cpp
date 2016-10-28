//
// Created by aLIEzTed on 10/24/16.
//

#include "../include/Lexer.h"
using namespace std;

void Lexer::init() {
  INT = Token("int", TokenType ::Int, "int");
  DOUBLE = Token("double", TokenType::Double, "double");
  FLOAT = Token("float", TokenType::Float, "float");
  LONG = Token("long", TokenType::Long, "long");
  CHAR = Token("char", TokenType::Char, "char");
  BREAK = Token("break", TokenType::Break, "break");
  SWITCH = Token("switch", TokenType::Switch, "switch");
  CASE = Token("case", TokenType::Case, "case");
  ELSE = Token("else", TokenType::Else, "else");
  ENUM = Token("enum", TokenType::Enum, "enum");
  IF = Token("if", TokenType::If, "if");
  RETURN = Token("return", TokenType::Return, "return");
  SIZEOF = Token("sizeof", TokenType::Sizeof, "sizeof");
  WHILE = Token("while", TokenType::While, "while");
  FOR = Token("for", TokenType::For, "for");
  VOID = Token("void", TokenType::Void, "void");
  MAIN = Token("main", TokenType::Main, "main");
  UNSIGNED = Token("unsigned", TokenType::Unsigned, "unsigned");
  DO = Token("do", TokenType::DO, "do");
  ASSIGN = Token("=", TokenType::Assign, "=");
  EQ = Token("==", TokenType::Eq, "==");
  NE = Token("!=", TokenType::Ne, "!=");
  INC = Token("++", TokenType::Inc, "++");
  DEC = Token("--", TokenType::Dec, "--");
  ADD = Token("+", TokenType::Add, "+");
  SUB = Token("-", TokenType::Sub, "-");
  MUL = Token("*", TokenType::Mul, "*");
  DIV = Token("/", TokenType::Div, "/");
  LE = Token("<=", TokenType::Le, "<=");
  LT = Token("<", TokenType::Lt, "<");
  GE = Token(">=", TokenType::Ge, ">=");
  GT = Token(">", TokenType::Gt, ">");
  NOT = Token("!", TokenType::Not, "!");
  LOR = Token("||", TokenType::L_or, "||");
  LAN = Token("&&", TokenType::L_an, "&&");
  MOD = Token("%", TokenType::Mod, "%");
  L_BRAK = Token("[", TokenType::L_BRAK, "[");
  R_BRAK = Token("]", TokenType::R_BRAK, "]");
  L_BR = Token("{", TokenType::L_BR, "{");
  R_BR = Token("}", TokenType::R_BR, "}");
  L_PH = Token("(", TokenType::L_PH, "(");
  R_PH = Token(")", TokenType::R_PH, ")");
  COND = Token("?", TokenType::Cond, "?");
  COLON = Token(":", TokenType::Colon, ":");
  S_COLON = Token(";", TokenType::S_Colon, ";");
  COMMA = Token(",", TokenType::Comma, ",");
  keywords["int"] = INT;
  keywords["do"] = DO;
  keywords["double"] = DOUBLE;
  keywords["float"] = FLOAT;
  keywords["long"] = LONG;
  keywords["unsigned"] = UNSIGNED;
  keywords["char"] = CHAR;
  keywords["break"] = BREAK;
  keywords["switch"] = SWITCH;
  keywords["if"] = IF;
  keywords["else"] = ELSE;
  keywords["for"] = FOR;
  keywords["while"] = WHILE;
  keywords["enum"] = ENUM;
  keywords["void"] = VOID;
  keywords["main"] = MAIN;
  keywords["return"] = RETURN;
  keywords["sizeof"] = SIZEOF;
  keywords["case"] = CASE;
}

Lexer::Lexer(const string &code) : code(code) {
  pos = 0;
  line = 0;
  init();
  initFunctions();
}

/*
 * Get all the tokens into token list.
 */
void Lexer::lexan() {
  while (pos < code.length()) {
    next();
  }
  if (pos != 0) {
    line++;
  }
}

void Lexer::push(Token *token) {
  curr_token = *token;
//  cout << "current token " << curr_token << ", at pos " << pos << endl;
  tokens.push_back(token);
}

void Lexer::next() {
  int last_pos;
  int token;
  while (1) {
    if (pos == code.length()) {
      break;
    }
    token = code[pos];
    ++pos;
    if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')) {
      // find a variable
      last_pos = pos - 1;
      while ((code[pos] >= 'a' && code[pos] <= 'z')
          || (code[pos] >= 'A' && code[pos] <= 'Z')
          || (code[pos] >= '0' && code[pos] <= '9')
          || (code[pos] == '_')) {
        pos++;
      }
      string name = code.substr((size_t)last_pos, (size_t)(pos - last_pos));
      if (keywords.find(name) != keywords.end()) {
        Token *t = &keywords.find(name)->second;
        push(t);
      } else {
        Token *t = new Token("Var", TokenType::Var, name);
        identifiers[name] = *t;
        push(t);
      }
    } else if (token >= '0' && token <= '9') {
      int token_val = token - '0';
      if (token_val > 0) {
        while (code[pos] >= '0' && code[pos] <= '9') {
          token_val = token_val * 10 + code[pos++] - '0';
        }
      } else {
        if (code[pos] == 'x' || code[pos] == 'X') {
          token = code[++pos];
          while ((token >= '0' && token <= '9') || (token >= 'a' && token <= 'f') || (token >= 'A' & token <= 'F')) {
            token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
            token = code[++pos];
          }
        } else {
          while (code[pos] >= '0' && code[pos] <= '7') {
            token_val = token_val * 8 + code[pos++] - '0';
          }
        }
      }
      Token *t = new Token("Num", TokenType::Num, to_string(token_val));
      push(t);

    } else if (token == '"' || token == '\'') {
      // TODO: deal with escaped string.
      int cur_pos = pos;
      while (pos < code.length()) {
        if (code[pos] == token) {
          break;
        }
        if (code[pos] == '\\') {
          pos++;
        }
        pos++;
      }
      if (code[pos] != token) {
        pos = cur_pos;
      }
      if (pos - cur_pos == 1) {
        Token *curr_token = new Token(string(1, code[cur_pos]), TokenType::Num, string(1, code[cur_pos]));
        push(curr_token);
      } else if (pos - cur_pos > 0) {
        string name = code.substr((size_t)cur_pos, (size_t)(pos - cur_pos));
        Token *t = new Token(name, TokenType::Str, name);
        push(t);
      }
    } else if (functions.find(token) != functions.end()) {
      functions.find(token)->second();
    }
  }
}


void Lexer::initFunctions() {
  auto comma = [this]() {
    push(&COMMA);
  };
  auto add = [this]() {
    if (code[pos] == '+') {
      pos++;
      push(&INC);
    } else {
      push(&ADD);
    }
  };
  auto sub = [this]() {
    if (code[pos] == '-') {
      pos++;
      push(&DEC);
    } else {
      push(&SUB);
    }
  };
  auto eq = [this]() {
    if (code[pos] == '=') {
      pos++;
      push(&EQ);
    } else {
      push(&ASSIGN);
    }
  };
  auto _not = [this]() {
    if (code[pos] == '=') {
      pos++;
      push(&NE);
    } else {
      push(&NOT);
    }
  };
  auto gt = [this]() {
    if (code[pos] == '=') {
      pos++;
      push(&GE);
    } else {
      push(&GT);
    }
  };
  auto div = [this]() {
    if (code[pos] == '/') {
      while (code[pos] != 0 && code[pos] != '\n') {
        ++pos;
      }
      if (code[pos] == '\n') {
        line++;
      }
    } else if (code[pos] == '*') {
      while (pos < code.length() && !(code[pos] == '*' && code[pos] == '/')) {
        if (code[pos] == '\n') {
          line++;
        }
        ++pos;
      }
    } else {
      push(&DIV);
    }
  };
  auto lt = [this]() {
    if (code[pos] == '=') {
      pos++;
      push(&LE);
    } else {
      push(&LT);
    }
  };
  auto colon = [this]() {
    push(&COLON);
  };
  auto s_colon = [this]() {
    push(&S_COLON);
  };
  auto r_ph = [this]() {
    push(&R_PH);
  };
  auto l_ph = [this]() {
    push(&L_PH);
  };
  auto r_br = [this]() {
    push(&R_BR);
  };
  auto l_br = [this]() {
    push(&L_BR);
  };
  auto l_brak = [this]() {
    push(&L_BRAK);
  };
  auto r_brak = [this]() {
    push(&R_BRAK);
  };
  auto cond = [this]() {
    push(&COND);
  };
  auto mul = [this]() {
    push(&MUL);
  };
  auto mod = [this]() {
    push(&MOD);
  };
  auto _and = [this]() {
    if (code[pos] == '&') {
      pos++;
      push(&LAN);
    }
  };
  auto _or = [this]() {
    if (code[pos] == '|') {
      pos++;
      push(&LOR);
    }
  };
  auto hash = [this]() {
    while (code[pos] != 0 && code[pos] != '\n') {
      pos++;
    }
  };
  auto n = [this]() {
    line++;
  };
  functions[','] = comma;
  functions['+'] = add;
  functions['-'] = sub;
  functions['/'] = div;
  functions['*'] = mul;
  functions['='] = eq;
  functions[':'] = colon;
  functions[';'] = s_colon;
  functions['?'] = cond;
  functions['('] = l_ph;
  functions[')'] = r_ph;
  functions['{'] = l_br;
  functions['}'] = r_br;
  functions['['] = l_brak;
  functions[']'] = r_brak;
  functions['%'] = mod;
  functions['&'] = _and;
  functions['|'] = _or;
  functions['>'] = gt;
  functions['<'] = lt;
  functions['!'] = _not;
  functions['\n'] = n;
  functions['#'] = hash;
}