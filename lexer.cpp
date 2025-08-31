#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

struct Token {
    string type;
    string lexeme;
};

vector<pair<string, regex>> TokensDefinaton = {
       {"T_INT",        regex("^int\\b")},
    {"T_FLOAT",      regex("^float\\b")},
    {"T_STRING",     regex("^string\\b")},
    {"T_BOOL",       regex("^bool\\b")},
    {"T_VOID",       regex("^void\\b")},

  
    {"T_FUNCTION",   regex("^fn\\b")},
    {"T_RETURN",     regex("^return\\b")},
    {"T_IF",         regex("^if\\b")},
    {"T_ELSE",       regex("^else\\b")},
    {"T_FOR",        regex("^for\\b")},
    {"T_WHILE",      regex("^while\\b")},
    {"T_BREAK",      regex("^break\\b")},
    {"T_CONTINUE",   regex("^continue\\b")},
    {"T_TRUE",       regex("^true\\b")},
    {"T_FALSE",      regex("^false\\b")},

    {"T_IDENTIFIER", regex("^[a-zA-Z][a-zA-Z0-9]*")},

    {"T_FLOATLIT",   regex("^[0-9]+\\.[0-9]+")},
    {"T_INTLIT",     regex("^[0-9]+")},

    {"T_PLUSOP",     regex("^\\+")},
    {"T_MINUSOP",    regex("^-")},
    {"T_MULTOP",     regex("^\\*")},
    {"T_DIVOP",      regex("^/")},
    {"T_MODOP",      regex("^%")},

    {"T_EQUALSOP",   regex("^==")},
    {"T_NOTEQOP",    regex("^!=")},
    {"T_LTEQOP",       regex("^<=")},
    {"T_GTEQOP",       regex("^>=")},
    {"T_ASSIGNOP",   regex("^=")},
    {"T_LTOP",         regex("^<")},
    {"T_GTOP",         regex("^>")},

    {"T_ANDOP",        regex("^&&")},
    {"T_OROP",         regex("^\\|\\|")},
    {"T_NOTOP",        regex("^!")},

    {"T_SHIFTLOP",     regex("^<<")},
    {"T_SHIFTROP",     regex("^>>")},
    {"T_BITANDOP",     regex("^&")},
    {"T_BITOROP",      regex("^\\|")},
    {"T_BITXOROP",     regex("^\\^")},
    {"T_BITNOTOP",     regex("^~")},

    {"T_SEMICOLON",  regex("^;")},
    {"T_COMMA",      regex("^,")},
    {"T_DOT",        regex("^\\.")},
    {"T_COLON",      regex("^:")},

    {"T_PARENL",     regex("^\\(")},
    {"T_PARENR",     regex("^\\)")},
    {"T_BRACEL",     regex("^\\{")},
    {"T_BRACER",     regex("^\\}")},
    {"T_SQUAREL",    regex("^\\[")},
    {"T_SQUARER",    regex("^\\]")},

    {"T_WHITESPACE", regex("^[ \t\n\r]+")},
    {"T_COMMENT_SINGLE", regex("^//.*")},
    {"T_COMMENT_MULTI",  regex("^/\\*[\\s\\S]*?\\*/")}
};

bool matchToken(const string& word, Token& matchToken) {
    for (auto &token : TokensDefinaton) {
        if (regex_match(word, token.second)) {
            matchToken = {token.first, word};
            return true;
        }
    }
    return false;
}

vector<Token> Toxenization(string input) {
    vector<Token> Tokens;
    string buffer = "";
    char c = ' ';

    for (int i = 0; i <= input.size(); i++) {
        char c = (i < input.size() ? input[i] : ' '); 
        if(i < input.size()){
            c = input[i];
        }else{
            c = ' ';
        }

        if (isspace(c) || c == ';' || c == ',' || c == '(' || c == ')' ||
            c == '{' || c == '}' || c == '[' || c == ']') {
            
            if (!buffer.empty()) {
                Token token;
                if (matchToken(buffer, token)) {
                    Tokens.push_back(token);
                } else {
                    cerr << "UNEXPECTED TOKEN: " << buffer << endl;
                }
                buffer.clear();
            }

            if (!isspace(c)) {
                string delimeter(1, c);
                Token token;
                if (matchToken(delimeter, token)) {
                    Tokens.push_back(token);
                }
            }
        } else {
            buffer += c;
        }
    }

    return Tokens;
}

int main() {
    string inputCode =
        R"(int x = 10;
        float y = 20.5;
        if (x < y) { return x; }
        else { return y; })";

    cout << "Input Code:\n" << inputCode << "\n\nTokens:\n";

    vector<Token> Tokens = Toxenization(inputCode);

    for (auto &token : Tokens) {
        cout << token.type << " -> " << token.lexeme << endl;
    }

    return 0;
}
