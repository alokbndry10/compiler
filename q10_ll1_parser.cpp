// Question 10: Write a program to implement LL(1) Parser

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
using namespace std;

struct Production {
    char lhs;
    string rhs;
    
    Production(char l, string r) : lhs(l), rhs(r) {}
};

class LL1Parser {
private:
    vector<Production> grammar;
    map<char, set<char>> first;
    map<char, set<char>> follow;
    map<pair<char, char>, int> parseTable; // [non-terminal][terminal] -> production index
    set<char> terminals;
    set<char> nonTerminals;
    char startSymbol;
    
public:
    LL1Parser(char start = 'S') : startSymbol(start) {}
    
    void addProduction(char lhs, string rhs) {
        grammar.push_back(Production(lhs, rhs));
        nonTerminals.insert(lhs);
        
        if (grammar.size() == 1) {
            startSymbol = lhs;
        }
        
        for (char c : rhs) {
            if (c >= 'A' && c <= 'Z') {
                nonTerminals.insert(c);
            } else if (c != 'ε') {
                terminals.insert(c);
            }
        }
    }
    
    void calculateFirst() {
        // Initialize FIRST sets
        for (char nt : nonTerminals) {
            first[nt] = set<char>();
        }
        
        bool changed = true;
        while (changed) {
            changed = false;
            
            for (const Production& prod : grammar) {
                char A = prod.lhs;
                string alpha = prod.rhs;
                
                if (alpha == "ε") {
                    if (first[A].find('ε') == first[A].end()) {
                        first[A].insert('ε');
                        changed = true;
                    }
                } else {
                    bool allDeriveEpsilon = true;
                    
                    for (size_t i = 0; i < alpha.length() && allDeriveEpsilon; i++) {
                        char X = alpha[i];
                        
                        if (terminals.find(X) != terminals.end()) {
                            if (first[A].find(X) == first[A].end()) {
                                first[A].insert(X);
                                changed = true;
                            }
                            allDeriveEpsilon = false;
                        } else {
                            for (char symbol : first[X]) {
                                if (symbol != 'ε' && first[A].find(symbol) == first[A].end()) {
                                    first[A].insert(symbol);
                                    changed = true;
                                }
                            }
                            
                            if (first[X].find('ε') == first[X].end()) {
                                allDeriveEpsilon = false;
                            }
                        }
                    }
                    
                    if (allDeriveEpsilon && first[A].find('ε') == first[A].end()) {
                        first[A].insert('ε');
                        changed = true;
                    }
                }
            }
        }
    }
    
    void calculateFollow() {
        // Initialize FOLLOW sets
        for (char nt : nonTerminals) {
            follow[nt] = set<char>();
        }
        
        follow[startSymbol].insert('$');
        
        bool changed = true;
        while (changed) {
            changed = false;
            
            for (const Production& prod : grammar) {
                char A = prod.lhs;
                string beta = prod.rhs;
                
                for (size_t i = 0; i < beta.length(); i++) {
                    char B = beta[i];
                    
                    if (nonTerminals.find(B) != nonTerminals.end()) {
                        string gamma = beta.substr(i + 1);
                        
                        // Calculate FIRST(γ)
                        set<char> firstGamma;
                        bool gammaDerivesEpsilon = true;
                        
                        for (char symbol : gamma) {
                            if (terminals.find(symbol) != terminals.end()) {
                                firstGamma.insert(symbol);
                                gammaDerivesEpsilon = false;
                                break;
                            } else {
                                for (char f : first[symbol]) {
                                    if (f != 'ε') {
                                        firstGamma.insert(f);
                                    }
                                }
                                if (first[symbol].find('ε') == first[symbol].end()) {
                                    gammaDerivesEpsilon = false;
                                    break;
                                }
                            }
                        }
                        
                        // Add FIRST(γ) - {ε} to FOLLOW(B)
                        for (char symbol : firstGamma) {
                            if (follow[B].find(symbol) == follow[B].end()) {
                                follow[B].insert(symbol);
                                changed = true;
                            }
                        }
                        
                        // If γ derives ε, add FOLLOW(A) to FOLLOW(B)
                        if (gammaDerivesEpsilon || gamma.empty()) {
                            for (char symbol : follow[A]) {
                                if (follow[B].find(symbol) == follow[B].end()) {
                                    follow[B].insert(symbol);
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    void constructParseTable() {
        for (size_t i = 0; i < grammar.size(); i++) {
            Production prod = grammar[i];
            char A = prod.lhs;
            string alpha = prod.rhs;
            
            // Calculate FIRST(α)
            set<char> firstAlpha;
            bool alphaDerivesEpsilon = true;
            
            if (alpha == "ε") {
                firstAlpha.insert('ε');
            } else {
                for (char symbol : alpha) {
                    if (terminals.find(symbol) != terminals.end()) {
                        firstAlpha.insert(symbol);
                        alphaDerivesEpsilon = false;
                        break;
                    } else {
                        for (char f : first[symbol]) {
                            if (f != 'ε') {
                                firstAlpha.insert(f);
                            }
                        }
                        if (first[symbol].find('ε') == first[symbol].end()) {
                            alphaDerivesEpsilon = false;
                            break;
                        }
                    }
                }
                if (alphaDerivesEpsilon) {
                    firstAlpha.insert('ε');
                }
            }
            
            // For each terminal a in FIRST(α), add A -> α to M[A, a]
            for (char a : firstAlpha) {
                if (a != 'ε') {
                    parseTable[{A, a}] = i;
                }
            }
            
            // If ε in FIRST(α), for each b in FOLLOW(A), add A -> α to M[A, b]
            if (firstAlpha.find('ε') != firstAlpha.end()) {
                for (char b : follow[A]) {
                    parseTable[{A, b}] = i;
                }
            }
        }
    }
    
    void printGrammar() {
        cout << "Grammar:" << endl;
        for (size_t i = 0; i < grammar.size(); i++) {
            cout << i + 1 << ". " << grammar[i].lhs << " -> " << grammar[i].rhs << endl;
        }
        cout << endl;
    }
    
    void printFirst() {
        cout << "FIRST sets:" << endl;
        for (const auto& pair : first) {
            cout << "FIRST(" << pair.first << ") = {";
            bool isFirst = true;
            for (char symbol : pair.second) {
                if (!isFirst) cout << ", ";
                cout << symbol;
                isFirst = false;
            }
            cout << "}" << endl;
        }
        cout << endl;
    }
    
    void printFollow() {
        cout << "FOLLOW sets:" << endl;
        for (const auto& pair : follow) {
            cout << "FOLLOW(" << pair.first << ") = {";
            bool isFirst = true;
            for (char symbol : pair.second) {
                if (!isFirst) cout << ", ";
                cout << symbol;
                isFirst = false;
            }
            cout << "}" << endl;
        }
        cout << endl;
    }
    
    void printParseTable() {
        cout << "LL(1) Parse Table:" << endl;
        cout << "NT\\T\t";
        for (char t : terminals) {
            cout << t << "\t";
        }
        cout << "$" << endl;
        
        for (char nt : nonTerminals) {
            cout << nt << "\t";
            for (char t : terminals) {
                auto it = parseTable.find({nt, t});
                if (it != parseTable.end()) {
                    cout << it->second + 1 << "\t";
                } else {
                    cout << "-\t";
                }
            }
            auto it = parseTable.find({nt, '$'});
            if (it != parseTable.end()) {
                cout << it->second + 1;
            } else {
                cout << "-";
            }
            cout << endl;
        }
        cout << endl;
    }
    
    bool parse(string input) {
        stack<char> parseStack;
        parseStack.push('$');
        parseStack.push(startSymbol);
        
        input += '$';
        size_t i = 0;
        
        cout << "Parsing steps for: " << input.substr(0, input.length()-1) << endl;
        cout << "Step\tStack\t\tInput\t\tAction" << endl;
        cout << string(50, '-') << endl;
        
        int step = 1;
        
        while (!parseStack.empty() && parseStack.top() != '$') {
            char top = parseStack.top();
            char currentInput = input[i];
            
            // Print current state
            stack<char> temp = parseStack;
            string stackStr = "";
            while (!temp.empty()) {
                stackStr = temp.top() + stackStr;
                temp.pop();
            }
            
            cout << step++ << "\t" << stackStr << "\t\t" << input.substr(i) << "\t\t";
            
            if (terminals.find(top) != terminals.end()) {
                // Terminal on top of stack
                if (top == currentInput) {
                    parseStack.pop();
                    i++;
                    cout << "Match " << top << endl;
                } else {
                    cout << "ERROR: Mismatch" << endl;
                    return false;
                }
            } else {
                // Non-terminal on top of stack
                auto it = parseTable.find({top, currentInput});
                if (it != parseTable.end()) {
                    int prodIndex = it->second;
                    Production prod = grammar[prodIndex];
                    
                    parseStack.pop();
                    
                    if (prod.rhs != "ε") {
                        for (int j = prod.rhs.length() - 1; j >= 0; j--) {
                            parseStack.push(prod.rhs[j]);
                        }
                    }
                    
                    cout << "Apply " << prod.lhs << " -> " << prod.rhs << endl;
                } else {
                    cout << "ERROR: No entry in parse table" << endl;
                    return false;
                }
            }
        }
        
        if (parseStack.top() == '$' && input[i] == '$') {
            cout << "SUCCESS: String accepted!" << endl;
            return true;
        } else {
            cout << "ERROR: String rejected!" << endl;
            return false;
        }
    }
};

int main() {
    cout << "LL(1) Parser Implementation" << endl;
    cout << "===========================" << endl;
    cout << "Enter grammar productions (Enter 'done' to finish)" << endl;
    cout << "Format: A->BC or A->a or A->ε" << endl;
    cout << "Use 'ε' for epsilon productions" << endl;
    cout << "First production's LHS will be the start symbol" << endl << endl;
    
    LL1Parser parser;
    string production;
    
    while (true) {
        cout << "Enter production: ";
        cin >> production;
        
        if (production == "done") break;
        
        // Parse production A->BC
        size_t arrowPos = production.find("->");
        if (arrowPos == string::npos) {
            cout << "Invalid format! Use A->BC" << endl;
            continue;
        }
        
        char lhs = production[0];
        string rhs = production.substr(arrowPos + 2);
        
        parser.addProduction(lhs, rhs);
        cout << "Added: " << lhs << " -> " << rhs << endl;
    }
    
    cout << "\n";
    parser.printGrammar();
    parser.calculateFirst();
    parser.printFirst();
    parser.calculateFollow();
    parser.printFollow();
    parser.constructParseTable();
    parser.printParseTable();
    
    string input;
    char choice;
    
    do {
        cout << "Enter string to parse: ";
        cin >> input;
        
        cout << "\n";
        bool result = parser.parse(input);
        
        cout << "\nDo you want to parse another string? (y/n): ";
        cin >> choice;
        cout << endl;
        
    } while (choice == 'y' || choice == 'Y');
    
    return 0;
}
