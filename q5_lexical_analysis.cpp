// Question 5: Write a program for lexical analysis

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <algorithm>
using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    DELIMITER,
    STRING_LITERAL,
    CHARACTER_LITERAL,
    COMMENT,
    UNKNOWN
};

struct Token {
    string value;
    TokenType type;
    int line;
    int column;
    
    Token(string val, TokenType t, int l, int c) : value(val), type(t), line(l), column(c) {}
};

class LexicalAnalyzer {
private:
    vector<string> keywords = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };
    
    vector<string> operators = {
        "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=",
        "&&", "||", "!", "&", "|", "^", "~", "<<", ">>", "++", "--",
        "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>="
    };
    
    vector<char> delimiters = {
        '(', ')', '{', '}', '[', ']', ';', ',', '.', ':', '?'
    };
    
    string input;
    int currentPos;
    int line;
    int column;
    vector<Token> tokens;
    
public:
    LexicalAnalyzer() : currentPos(0), line(1), column(1) {}
    
    bool isKeyword(const string& word) {
        return find(keywords.begin(), keywords.end(), word) != keywords.end();
    }
    
    bool isOperator(const string& op) {
        return find(operators.begin(), operators.end(), op) != operators.end();
    }
    
    bool isDelimiter(char ch) {
        return find(delimiters.begin(), delimiters.end(), ch) != delimiters.end();
    }
    
    string getTokenTypeName(TokenType type) {
        switch (type) {
            case KEYWORD: return "KEYWORD";
            case IDENTIFIER: return "IDENTIFIER";
            case NUMBER: return "NUMBER";
            case OPERATOR: return "OPERATOR";
            case DELIMITER: return "DELIMITER";
            case STRING_LITERAL: return "STRING_LITERAL";
            case CHARACTER_LITERAL: return "CHARACTER_LITERAL";
            case COMMENT: return "COMMENT";
            case UNKNOWN: return "UNKNOWN";
            default: return "UNKNOWN";
        }
    }
    
    void skipWhitespace() {
        while (currentPos < input.length() && isspace(input[currentPos])) {
            if (input[currentPos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            currentPos++;
        }
    }
    
    Token readNumber() {
        int startCol = column;
        string number = "";
        
        while (currentPos < input.length() && 
               (isdigit(input[currentPos]) || input[currentPos] == '.')) {
            number += input[currentPos];
            currentPos++;
            column++;
        }
        
        return Token(number, NUMBER, line, startCol);
    }
    
    Token readIdentifierOrKeyword() {
        int startCol = column;
        string word = "";
        
        while (currentPos < input.length() && 
               (isalnum(input[currentPos]) || input[currentPos] == '_')) {
            word += input[currentPos];
            currentPos++;
            column++;
        }
        
        TokenType type = isKeyword(word) ? KEYWORD : IDENTIFIER;
        return Token(word, type, line, startCol);
    }
    
    Token readStringLiteral() {
        int startCol = column;
        string str = "";
        char quote = input[currentPos];
        str += quote;
        currentPos++;
        column++;
        
        while (currentPos < input.length() && input[currentPos] != quote) {
            if (input[currentPos] == '\\' && currentPos + 1 < input.length()) {
                str += input[currentPos++];
                column++;
            }
            str += input[currentPos];
            currentPos++;
            column++;
        }
        
        if (currentPos < input.length()) {
            str += input[currentPos];
            currentPos++;
            column++;
        }
        
        TokenType type = (quote == '"') ? STRING_LITERAL : CHARACTER_LITERAL;
        return Token(str, type, line, startCol);
    }
    
    Token readOperator() {
        int startCol = column;
        string op = "";
        
        // Try to read two-character operators first
        if (currentPos + 1 < input.length()) {
            string twoChar = input.substr(currentPos, 2);
            if (isOperator(twoChar)) {
                op = twoChar;
                currentPos += 2;
                column += 2;
                return Token(op, OPERATOR, line, startCol);
            }
        }
        
        // Read single-character operator
        op = input[currentPos];
        currentPos++;
        column++;
        
        return Token(op, OPERATOR, line, startCol);
    }
    
    Token readComment() {
        int startCol = column;
        string comment = "";
        
        if (input.substr(currentPos, 2) == "//") {
            // Single-line comment
            while (currentPos < input.length() && input[currentPos] != '\n') {
                comment += input[currentPos];
                currentPos++;
                column++;
            }
        } else if (input.substr(currentPos, 2) == "/*") {
            // Multi-line comment
            comment += input[currentPos++];
            comment += input[currentPos++];
            column += 2;
            
            while (currentPos + 1 < input.length()) {
                if (input[currentPos] == '*' && input[currentPos + 1] == '/') {
                    comment += input[currentPos++];
                    comment += input[currentPos++];
                    column += 2;
                    break;
                }
                if (input[currentPos] == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
                comment += input[currentPos++];
            }
        }
        
        return Token(comment, COMMENT, line, startCol);
    }
    
    void analyze(const string& sourceCode) {
        input = sourceCode;
        currentPos = 0;
        line = 1;
        column = 1;
        tokens.clear();
        
        while (currentPos < input.length()) {
            skipWhitespace();
            
            if (currentPos >= input.length()) break;
            
            char ch = input[currentPos];
            
            // Check for comments
            if (currentPos + 1 < input.length() && 
                (input.substr(currentPos, 2) == "//" || input.substr(currentPos, 2) == "/*")) {
                tokens.push_back(readComment());
            }
            // Check for string/character literals
            else if (ch == '"' || ch == '\'') {
                tokens.push_back(readStringLiteral());
            }
            // Check for numbers
            else if (isdigit(ch)) {
                tokens.push_back(readNumber());
            }
            // Check for identifiers/keywords
            else if (isalpha(ch) || ch == '_') {
                tokens.push_back(readIdentifierOrKeyword());
            }
            // Check for delimiters
            else if (isDelimiter(ch)) {
                tokens.push_back(Token(string(1, ch), DELIMITER, line, column));
                currentPos++;
                column++;
            }
            // Check for operators
            else if (ispunct(ch)) {
                tokens.push_back(readOperator());
            }
            // Unknown character
            else {
                tokens.push_back(Token(string(1, ch), UNKNOWN, line, column));
                currentPos++;
                column++;
            }
        }
    }
    
    void printTokens() {
        cout << "\nLexical Analysis Results:" << endl;
        cout << "=========================" << endl;
        cout << "Line\tCol\tToken\t\tType" << endl;
        cout << "----\t---\t-----\t\t----" << endl;
        
        for (const Token& token : tokens) {
            cout << token.line << "\t" << token.column << "\t";
            cout << token.value;
            
            // Add padding for alignment
            if (token.value.length() < 8) {
                cout << "\t\t";
            } else {
                cout << "\t";
            }
            
            cout << getTokenTypeName(token.type) << endl;
        }
    }
    
    void printStatistics() {
        map<TokenType, int> count;
        for (const Token& token : tokens) {
            count[token.type]++;
        }
        
        cout << "\nToken Statistics:" << endl;
        cout << "=================" << endl;
        for (const auto& pair : count) {
            cout << getTokenTypeName(pair.first) << ": " << pair.second << endl;
        }
        cout << "Total Tokens: " << tokens.size() << endl;
    }
};

int main() {
    cout << "Lexical Analyzer for C/C++ Code" << endl;
    cout << "================================" << endl;
    cout << "Enter source code (type 'END' on a new line to finish):" << endl;
    
    LexicalAnalyzer analyzer;
    string sourceCode = "";
    string line;
    
    while (getline(cin, line)) {
        if (line == "END") break;
        sourceCode += line + "\n";
    }
    
    if (!sourceCode.empty()) {
        analyzer.analyze(sourceCode);
        analyzer.printTokens();
        analyzer.printStatistics();
    } else {
        cout << "No source code provided!" << endl;
    }
    
    return 0;
}
