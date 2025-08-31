#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

struct Token {
    string type;
    string lexeme;
};

vector<string> keywords = {
    "int", "float", "string", "bool", "void",
    "fn", "return", "if", "else", "for", "while",
    "break", "continue", "true", "false"
};

vector<pair<string, string>> operators = {
    {"T_EQUALSOP", "=="},
    {"T_NOTEQOP", "!="},
    {"T_LTEQOP", "<="},
    {"T_GTEQOP", ">="},
    {"T_SHIFTLOP", "<<"},
    {"T_SHIFTROP", ">>"},
    {"T_ANDOP", "&&"},
    {"T_OROP", "||"},

    {"T_PLUSOP", "+"},
    {"T_MINUSOP", "-"},
    {"T_MULTOP", "*"},
    {"T_DIVOP", "/"},
    {"T_MODOP", "%"},
    {"T_ASSIGNOP", "="},
    {"T_LTOP", "<"},
    {"T_GTOP", ">"},
    {"T_NOTOP", "!"},
    {"T_BITANDOP", "&"},
    {"T_BITOROP", "|"},
    {"T_BITXOROP", "^"},
    {"T_BITNOTOP", "~"},
};

vector<pair<string, string>> delimiters = {
    {"T_SEMICOLON", ";"},
    {"T_COMMA", ","},
    {"T_DOT", "."},
    {"T_COLON", ":"},
    {"T_PARENL", "("},
    {"T_PARENR", ")"},
    {"T_BRACEL", "{"},
    {"T_BRACER", "}"},
    {"T_SQUAREL", "["},
    {"T_SQUARER", "]"}
};

bool isKeyword(const string& s) {
    for (auto& k : keywords)
        if (s == k) return true;
    return false;
}

bool isIdentifier(const string& s) {
    if (s.empty()) return false;
    if (!isalpha(s[0]) && s[0] != '_') return false;
    for (char c : s)
        if (!isalnum(c) && c != '_') return false;
    return true;
}

vector<Token> tokenize(const string& input) {
    vector<Token> tokens;
    int i = 0, n = input.size();

    while (i < n) {
        char c = input[i];


        if (isspace(c)) { i++; continue; }


        if (c == '/' && i+1 < n && input[i+1] == '/') {
            while (i < n && input[i] != '\n') i++;
            continue;
        }


        if (c == '/' && i+1 < n && input[i+1] == '*') {
            i += 2;
            while (i+1 < n && !(input[i] == '*' && input[i+1] == '/')) i++;
            i += 2;
            continue;
        }


        if (isdigit(c)) {
            string num;
            num += c;
            i++;
            while (i < n && isdigit(input[i])) {
                num += input[i++];
            }


            if (i < n && input[i] == '.') {
                num += input[i++];
                while (i < n && isdigit(input[i])) {
                    num += input[i++];
                }
                tokens.push_back({"T_FLOATLIT", num});
            }

            else if (i < n && (isalpha(input[i]) || input[i] == '_')) {
                string invalid = num;
                while (i < n && (isalnum(input[i]) || input[i] == '_')) {
                    invalid += input[i++];
                }
                cerr << "LEXICAL ERROR: Invalid identifier '" << invalid
                     << "' (cannot start with digit)\n";
                exit(1);
            }
            else {
                tokens.push_back({"T_INTLIT", num});
            }
            continue;
        }


        if (isalpha(c) || c == '_') {
            string word;
            while (i < n && (isalnum(input[i]) || input[i] == '_')) {
                word += input[i++];
            }
            if (isKeyword(word))
                tokens.push_back({"T_" + string(1, toupper(word[0])) + word.substr(1), word});
            else
                tokens.push_back({"T_IDENTIFIER", word});
            continue;
        }


        bool matched = false;
        for (auto& op : operators) {
            int len = op.second.size();
            if (i+len <= n && input.substr(i, len) == op.second) {
                tokens.push_back({op.first, op.second});
                i += len;
                matched = true;
                break;
            }
        }
        if (matched) continue;


        for (auto& d : delimiters) {
            if (c == d.second[0]) {
                tokens.push_back({d.first, d.second});
                i++;
                matched = true;
                break;
            }
        }
        if (matched) continue;


        cerr << "UNEXPECTED TOKEN: " << c << endl;
        i++;
    }

    return tokens;
}

int main() {
    string inputCode =
        R"(int 9x = 10;
        float y = 20.5;
        if (x < y) { return x; }
        else { return y; })";

    cout << "Input Code:\n" << inputCode << "\n\nTokens:\n";

    vector<Token> Tokens = tokenize(inputCode);

    for (auto &token : Tokens) {
        cout << token.type << " -> " << token.lexeme << endl;
    }
}
