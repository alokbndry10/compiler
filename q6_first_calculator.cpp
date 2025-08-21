// Question 6: Write a program to calculate FIRST of any given grammar

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

struct Production {
    char lhs;
    string rhs;
    
    Production(char l, string r) : lhs(l), rhs(r) {}
};

class FirstCalculator {
private:
    vector<Production> productions;
    map<char, set<char>> first;
    set<char> terminals;
    set<char> non_terminals;
    
public:
    void addProduction(char lhs, string rhs) {
        productions.push_back(Production(lhs, rhs));
        non_terminals.insert(lhs);
        
        for (char c : rhs) {
            if (c >= 'A' && c <= 'Z') {
                non_terminals.insert(c);
            } else if (c != 'ε') {
                terminals.insert(c);
            }
        }
    }
    
    void calculateFirst() {
        bool changed = true;
        
        // Initialize FIRST sets
        for (char nt : non_terminals) {
            first[nt] = set<char>();
        }
        
        while (changed) {
            changed = false;
            
            for (const Production& prod : productions) {
                char lhs = prod.lhs;
                string rhs = prod.rhs;
                
                if (rhs == "ε") {
                    // If A -> ε, add ε to FIRST(A)
                    if (first[lhs].find('ε') == first[lhs].end()) {
                        first[lhs].insert('ε');
                        changed = true;
                    }
                } else {
                    // For A -> X1 X2 ... Xn
                    bool all_nullable = true;
                    
                    for (size_t i = 0; i < rhs.length() && all_nullable; i++) {
                        char X = rhs[i];
                        
                        if (terminals.find(X) != terminals.end()) {
                            // X is terminal
                            if (first[lhs].find(X) == first[lhs].end()) {
                                first[lhs].insert(X);
                                changed = true;
                            }
                            all_nullable = false;
                        } else {
                            // X is non-terminal
                            set<char> first_X = first[X];
                            
                            // Add FIRST(X) - {ε} to FIRST(A)
                            for (char symbol : first_X) {
                                if (symbol != 'ε' && first[lhs].find(symbol) == first[lhs].end()) {
                                    first[lhs].insert(symbol);
                                    changed = true;
                                }
                            }
                            
                            // If ε not in FIRST(X), stop
                            if (first_X.find('ε') == first_X.end()) {
                                all_nullable = false;
                            }
                        }
                    }
                    
                    // If all symbols are nullable, add ε to FIRST(A)
                    if (all_nullable && first[lhs].find('ε') == first[lhs].end()) {
                        first[lhs].insert('ε');
                        changed = true;
                    }
                }
            }
        }
    }
    
    void printGrammar() {
        cout << "Grammar Productions:" << endl;
        for (const Production& prod : productions) {
            cout << prod.lhs << " -> " << prod.rhs << endl;
        }
        cout << endl;
    }
    
    void printFirst() {
        cout << "FIRST Sets:" << endl;
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
    }
    
    set<char> getFirst(char symbol) {
        return first[symbol];
    }
};

int main() {
    cout << "FIRST Set Calculator for Context-Free Grammars" << endl;
    cout << "===============================================" << endl;
    
    FirstCalculator calc;
    
    // Example Grammar 1: Simple grammar
    cout << "Example 1:" << endl;
    calc.addProduction('S', "AB");
    calc.addProduction('A', "a");
    calc.addProduction('A', "ε");
    calc.addProduction('B', "b");
    calc.addProduction('B', "c");
    
    calc.printGrammar();
    calc.calculateFirst();
    calc.printFirst();
    
    cout << "\n" << string(50, '=') << "\n" << endl;
    
    // Example Grammar 2: More complex grammar
    cout << "Example 2:" << endl;
    FirstCalculator calc2;
    calc2.addProduction('E', "TX");
    calc2.addProduction('X', "+TX");
    calc2.addProduction('X', "ε");
    calc2.addProduction('T', "FY");
    calc2.addProduction('Y', "*FY");
    calc2.addProduction('Y', "ε");
    calc2.addProduction('F', "(E)");
    calc2.addProduction('F', "id");
    
    calc2.printGrammar();
    calc2.calculateFirst();
    calc2.printFirst();
    
    cout << "\n" << string(50, '=') << "\n" << endl;
    
    // Example Grammar 3: Grammar with left recursion
    cout << "Example 3:" << endl;
    FirstCalculator calc3;
    calc3.addProduction('S', "Sa");
    calc3.addProduction('S', "b");
    
    calc3.printGrammar();
    calc3.calculateFirst();
    calc3.printFirst();
    
    return 0;
}
