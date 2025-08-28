#include <iostream>
#include <vector>
#include <string>
#include <cctype> 
using namespace std;


enum TokenType {
    T_FUNCTION,   // "fn"
    T_INT,        // "int"
    T_FLOAT,      // "float"
    T_BOOL,       // "bool"
    T_STRING,     // "string"
    T_RETURN,     // "return"

    T_IDENTIFIER, // variable/function names
    T_INTLIT,     // integer literals (e.g., 123)
    T_STRINGLIT,  // string literals (e.g., "hello")

    // Symbols & operators
    T_PARENL,     // (
    T_PARENR,     // )
    T_BRACEL,     // {
    T_BRACER,     // }
    T_EQUALSOP,   // ==
    T_ASSIGNOP,   // =
    T_SEMICOLON,  // ;
    T_COMMA,      // ,

    T_UNKNOWN,    // anything invalid/unrecognized
    T_EOF         // end of file/input
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
        case T_UNKNOWN: return "T_UNKNOWN";
        case T_EOF: return "T_EOF";
        default: return "T_UNKNOWN";
    }
}

class SimpleLexer {
    string code;             
    vector<Token> tokens;    
    size_t pos = 0;          

    char peek() { return pos < code.size() ? code[pos] : '\0'; }

    char get() { return pos < code.size() ? code[pos++] : '\0'; }

    void skipWhitespace() {
        while (isspace(peek())) get();
    }

public:
    SimpleLexer(string src) : code(src) {}

    void tokenize() {
        while (pos < code.size()) {
            skipWhitespace();
            char c = peek(); 

            if (isalpha(c) || c == '_') {
                string word;
                while (isalnum(peek()) || peek() == '_') 
                    word += get();

                if (word == "fn") tokens.push_back({T_FUNCTION, word});
                else if (word == "int") tokens.push_back({T_INT, word});
                else if (word == "float") tokens.push_back({T_FLOAT, word});
                else if (word == "bool") tokens.push_back({T_BOOL, word});
                else if (word == "string") tokens.push_back({T_STRING, word});
                else if (word == "return") tokens.push_back({T_RETURN, word});
                else tokens.push_back({T_IDENTIFIER, word});
            }

            else if (isdigit(c)) {
                string num;
                while (isdigit(peek())) num += get();
                tokens.push_back({T_INTLIT, num});
            }

            else if (c == '"') {
                string str;
                get(); 
                while (peek() != '"' && peek() != '\0') {
                    if (peek() == '\\') { 
                        str += get(); 
                        str += get(); 
                    } else {
                        str += get();
                    }
                }
                get(); 
                tokens.push_back({T_STRINGLIT, str});
            }

            else {
                switch (c) {
                    case '=':
                        get();
                        if (peek() == '=') { 
                            get();
                            tokens.push_back({T_EQUALSOP, "=="});
                        } else {
                            tokens.push_back({T_ASSIGNOP, "="});
                        }
                        break;
                    case '(': get(); tokens.push_back({T_PARENL, "("}); break;
                    case ')': get(); tokens.push_back({T_PARENR, ")"}); break;
                    case '{': get(); tokens.push_back({T_BRACEL, "{"}); break;
                    case '}': get(); tokens.push_back({T_BRACER, "}"}); break;
                    case ';': get(); tokens.push_back({T_SEMICOLON, ";"}); break;
                    case ',': get(); tokens.push_back({T_COMMA, ","}); break;
                    default:
                        
                        tokens.push_back({T_UNKNOWN, string(1, get())});
                }
            }
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
    string code = R"(fn int my_fn(int x, float y) {
        string my_str = "hmm";
        bool my_bool = x == 40;
        return x;
    })";

    SimpleLexer lexer(code);
    lexer.tokenize();
    lexer.printTokens();

    return 0;
}
