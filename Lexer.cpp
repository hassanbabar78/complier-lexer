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

vector<Token> Toxenization(string input) {
    vector<Token> Tokens;
    while (!input.empty()) {
        bool isMatched = false;
        for (auto &token : TokensDefinaton) {
            smatch match;
            if (regex_search(input, match, token.second)) {
                string matchedLexeme = match.str();
                if (token.first != "T_WHITESPACE" && 
                    token.first != "T_COMMENT_SINGLE" && 
                    token.first != "T_COMMENT_MULTI") {
                    Tokens.push_back({token.first, matchedLexeme});
                }
                input = input.substr(matchedLexeme.size());
                isMatched = true;
                break;
            }
        }
        if (!isMatched) {
            cerr << "UNEXPEXTED TOKEN: this is now Allowed... " << input[0] << endl;
            input = input.substr(1);
        }
    }
    return Tokens;
}

int main() {
    string inputCode =
        R"(int 0x = 10
        float y = 20.5
        if (x < y) { return x }
        else { return y})";

    cout << "Input Code:\n" << inputCode << "\n\nTokens:\n";

    vector<Token> Tokens = Toxenization(inputCode);

    for (auto &token : Tokens) {
        cout << token.type << " -> " << token.lexeme << endl;
    }

    return 0;
}
