#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <stdexcept>
#include <unordered_map>
using namespace std;
enum TokenType {
    T_FUNCTION, T_INT, T_FLOAT, T_STRING, T_BOOL, T_RETURN, T_IF, T_ELSE,
    T_IDENTIFIER, T_INTLIT, T_FLOATLIT, T_STRINGLIT,
    T_ASSIGNOP, T_EQUALSOP, T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_LT, T_GT, T_LTE, T_GTE, T_NEQ, T_AND, T_OR,
    T_PARENL, T_PARENR, T_BRACEL, T_BRACER, T_COMMA, T_SEMICOLON,
    T_QUOTES, T_UNKNOWN
};

struct Token {
    TokenType type;
    string value;
};


unordered_map<TokenType, string> tokenNames = {
    {T_FUNCTION, "TFUNCTION"}, {T_INT, "TINT"}, {T_FLOAT, "TFLOAT"}, {T_STRING, "TSTRING"},
    {T_BOOL, "TBOOL"}, {T_RETURN, "TRETURN"}, {T_IF, "TIF"}, {T_ELSE, "TELSE"},
    {T_IDENTIFIER, "TIDENTIFIER"}, {T_INTLIT, "TINTLIT"}, {T_FLOATLIT, "TFLOATLIT"},
    {T_STRINGLIT, "TSTRINGLIT"}, {T_ASSIGNOP, "TASSIGNOP"}, {T_EQUALSOP, "TEQUALSOP"},
    {T_PLUS, "TPLUS"}, {T_MINUS, "TMINUS"}, {T_MUL, "TMUL"}, {T_DIV, "TDIV"},
    {T_LT, "TLTOP"}, {T_GT, "TGTOP"}, {T_LTE, "TLTE"}, {T_GTE, "TGTE"}, {T_NEQ, "TNEQ"},
    {T_AND, "TAND"}, {T_OR, "TOR"}, {T_PARENL, "TPARENL"}, {T_PARENR, "TPARENR"},
    {T_BRACEL, "TBRACEL"}, {T_BRACER, "TBRACER"}, {T_COMMA, "TCOMMA"}, {T_SEMICOLON, "TSEMICOLON"},
    {T_QUOTES, "TQUOTES"}, {T_UNKNOWN, "TUNKNOWN"}
};

class Lexer {
    string src;
    size_t pos = 0;

    char peek() {
        return (pos < src.size()) ? src[pos] : '\0';
    }

    char get() {
        return (pos < src.size()) ? src[pos++] : '\0';
    }

    void skipWhitespace() {
        while (isspace(peek())) get();
    }

    bool isIdentifierStart(char c) {
        return isalpha(c) || c == '_' || (c >= 128);
    }

    bool isIdentifierChar(char c) {
        return isalnum(c) || c == '_' || (c >= 128);
    }

public:
    Lexer(const string &input) : src(input) {}

    vector<Token> tokenize() {
        vector<Token> tokens;

        while (pos < src.size()) {
            skipWhitespace();
            char c = peek();
            if (!c) break;


            if (isIdentifierStart(c)) {
                string ident;
                while (isIdentifierChar(peek())) ident += get();

                if (ident == "fn") tokens.push_back({T_FUNCTION, ident});
                else if (ident == "int") tokens.push_back({T_INT, ident});
                else if (ident == "float") tokens.push_back({T_FLOAT, ident});
                else if (ident == "string") tokens.push_back({T_STRING, ident});
                else if (ident == "bool") tokens.push_back({T_BOOL, ident});
                else if (ident == "return") tokens.push_back({T_RETURN, ident});
                else if (ident == "if") tokens.push_back({T_IF, ident});
                else if (ident == "else") tokens.push_back({T_ELSE, ident});
                else tokens.push_back({T_IDENTIFIER, ident});
                continue;
            }


            if (isdigit(c)) {
                std::string num;
                bool isFloat = false;
                while (isdigit(peek()) || peek() == '.') {
                    if (peek() == '.') {
                        if (isFloat) throw runtime_error("Invalid float literal");
                        isFloat = true;
                    }
                    num += get();
                }
                if (isFloat) tokens.push_back({T_FLOATLIT, num});
                else tokens.push_back({T_INTLIT, num});
                continue;
            }


            if (c == '"') {
                get();
                std::string str;
                while (peek() != '"' && peek() != '\0') {
                    if (peek() == '\\') {
                        get();
                        char esc = get();
                        if (esc == 'n') str += "\\n";
                        else if (esc == 't') str += "\\t";
                        else if (esc == '"') str += '"';
                        else str += esc;
                    } else str += get();
                }
                if (peek() == '"') get();
                tokens.push_back({T_STRINGLIT, str});
                continue;
            }


            switch (c) {
                case '=': get(); if (peek() == '=') { get(); tokens.push_back({T_EQUALSOP, "=="}); } 
                          else tokens.push_back({T_ASSIGNOP, "="}); continue;
                case '+': get(); tokens.push_back({T_PLUS, "+"}); continue;
                case '-': get(); tokens.push_back({T_MINUS, "-"}); continue;
                case '*': get(); tokens.push_back({T_MUL, "*"}); continue;
                case '/': get(); tokens.push_back({T_DIV, "/"}); continue;
                case '<': get(); tokens.push_back({T_LT, "<"}); continue;
                case '>': get(); tokens.push_back({T_GT, ">"}); continue;
                case '!': get(); if (peek() == '=') { get(); tokens.push_back({T_NEQ, "!="}); } 
                          else throw runtime_error("Unexpected !"); continue;
                case '&': get(); if (peek() == '&') { get(); tokens.push_back({T_AND, "&&"}); } 
                          else throw runtime_error("Unexpected &"); continue;
                case '|': get(); if (peek() == '|') { get(); tokens.push_back({T_OR, "||"}); } 
                          else throw runtime_error("Unexpected |"); continue;
                case '(': get(); tokens.push_back({T_PARENL, "("}); continue;
                case ')': get(); tokens.push_back({T_PARENR, ")"}); continue;
                case '{': get(); tokens.push_back({T_BRACEL, "{"}); continue;
                case '}': get(); tokens.push_back({T_BRACER, "}"}); continue;
                case ',': get(); tokens.push_back({T_COMMA, ","}); continue;
                case ';': get(); tokens.push_back({T_SEMICOLON, ";"}); continue;
                default:
                    throw runtime_error(std::string("Unexpected character: ") + c);
            }
        }
        return tokens;
    }
};

int main() {
    string code = R"(
        int x = 10;
        float y = 20.5;
        if (x < y) {
            return x;
        } else {
            return y;
        }
    )";

    Lexer lexer(code);
    try {
        auto tokens = lexer.tokenize();
        cout << "Tokens:\n\n";
        for (auto &tok : tokens) {
            cout << tokenNames[tok.type] << " -> " << tok.value << "\n";
        }
    } catch (std::exception &e) {
        cerr << "Lexer error: " << e.what() << "\n";
    }
}
