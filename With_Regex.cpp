#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cctype>

using namespace std;

enum TokenType {
    T_FUNCTION, T_INT, T_FLOAT, T_BOOL, T_STRING, T_RETURN,
    T_IDENTIFIER, T_INTLIT, T_STRINGLIT,
    T_PARENL, T_PARENR, T_BRACEL, T_BRACER,
    T_EQUALSOP, T_ASSIGNOP, T_SEMICOLON, T_COMMA,
    T_EOF
};

struct Token {
    TokenType type;
    string value;
};

string tokenName(TokenType t) {
    switch (t) {
        case T_FUNCTION: return "T_FUNCTION";
        case T_INT: return "T_INT";
        case T_FLOAT: return "T_FLOAT";
        case T_BOOL: return "T_BOOL";
        case T_STRING: return "T_STRING";
        case T_RETURN: return "T_RETURN";
        case T_IDENTIFIER: return "T_IDENTIFIER";
        case T_INTLIT: return "T_INTLIT";
        case T_STRINGLIT: return "T_STRINGLIT";
        case T_PARENL: return "T_PARENL";
        case T_PARENR: return "T_PARENR";
        case T_BRACEL: return "T_BRACEL";
        case T_BRACER: return "T_BRACER";
        case T_EQUALSOP: return "T_EQUALSOP";
        case T_ASSIGNOP: return "T_ASSIGNOP";
        case T_SEMICOLON: return "T_SEMICOLON";
        case T_COMMA: return "T_COMMA";
        case T_EOF: return "T_EOF";
        default: return "UNKNOWN";
    }
}

class RegexLexer {
    string code;
    vector<Token> tokens;

public:
    RegexLexer(string src) : code(src) {}

    void tokenize() {
        regex tokenPatterns(
            R"((fn)|"                    // function
               (int)|"                   // int
               (float)|"                 // float
               (bool)|"                  // bool
               (string)|"                // string
               (return)|"                // return
               ([a-zA-Z_][a-zA-Z0-9_]*)|" // identifiers
               (\d+)|"                   // integer literal
               ("(\\.|[^"])*")|"         // string literal
               (\()|(\))|(\{)|(\})|"     // parentheses / braces
               (==)|"                    // equality operator
               (=)|"                     // assignment operator
               (;)|"                     // semicolon
               (,))",                    // comma
            regex::extended
        );

        auto words_begin = sregex_iterator(code.begin(), code.end(), tokenPatterns);
        auto words_end = sregex_iterator();

        for (auto i = words_begin; i != words_end; ++i) {
            smatch match = *i;
            string m = match.str();

            if (match[1].matched) tokens.push_back({T_FUNCTION, m});
            else if (match[2].matched) tokens.push_back({T_INT, m});
            else if (match[3].matched) tokens.push_back({T_FLOAT, m});
            else if (match[4].matched) tokens.push_back({T_BOOL, m});
            else if (match[5].matched) tokens.push_back({T_STRING, m});
            else if (match[6].matched) tokens.push_back({T_RETURN, m});
            else if (match[7].matched) tokens.push_back({T_IDENTIFIER, m});
            else if (match[8].matched) tokens.push_back({T_INTLIT, m});
            else if (match[9].matched) tokens.push_back({T_STRINGLIT, m});
            else if (match[10].matched) tokens.push_back({T_PARENL, m});
            else if (match[11].matched) tokens.push_back({T_PARENR, m});
            else if (match[12].matched) tokens.push_back({T_BRACEL, m});
            else if (match[13].matched) tokens.push_back({T_BRACER, m});
            else if (match[14].matched) tokens.push_back({T_EQUALSOP, m});
            else if (match[15].matched) tokens.push_back({T_ASSIGNOP, m});
            else if (match[16].matched) tokens.push_back({T_SEMICOLON, m});
            else if (match[17].matched) tokens.push_back({T_COMMA, m});
        }

        tokens.push_back({T_EOF, ""});
    }

    void printTokens() {
        for (auto &t : tokens) {
            cout << "[" << tokenName(t.type);
            if (!t.value.empty()) cout << "(\"" << t.value << "\")";
            cout << "] ";
        }
        cout << endl;
    }
};

int main() {
    string code = R"(
        fn int my_fn(int x, float y) {
            string my_str = "hello";
            bool my_bool = x == 40;
            return x;
        }
    )";

    RegexLexer lexer(code);
    lexer.tokenize();   
    lexer.printTokens();

    return 0;
}
