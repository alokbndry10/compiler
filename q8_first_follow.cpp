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

class FirstFollowCalculator {
private:
    vector<Production> productions;
    map<char, set<char>> first;
    map<char, set<char>> follow;
    set<char> non_terminals;
    set<char> terminals;
    char start_symbol;
    
public:
    FirstFollowCalculator() : start_symbol('\0') {}
    
    void addProduction(char lhs, string rhs) {
        productions.push_back(Production(lhs, rhs));
        non_terminals.insert(lhs);
        
        if (start_symbol == '\0') {
            start_symbol = lhs;
        }
        
        for (char c : rhs) {
            if (c != '#' && (c < 'A' || c > 'Z')) {
                terminals.insert(c);
            }
        }
    }
    
    void calculateFirst() {
        cout << "\nCalculating FIRST sets..." << endl;
        
        for (char nt : non_terminals) {
            first[nt] = set<char>();
        }
        
        bool changed = true;
        while (changed) {
            changed = false;
            
            for (const Production& prod : productions) {
                char A = prod.lhs;
                string alpha = prod.rhs;
                
                if (alpha == "#") {
                    if (first[A].find('#') == first[A].end()) {
                        first[A].insert('#');
                        changed = true;
                    }
                } else {
                    int k = 0;
                    bool canDeriveEpsilon = true;
                    
                    for (char X : alpha) {
                        if (X < 'A' || X > 'Z') {
                            if (first[A].find(X) == first[A].end()) {
                                first[A].insert(X);
                                changed = true;
                            }
                            canDeriveEpsilon = false;
                            break;
                        } else {
                            for (char symbol : first[X]) {
                                if (symbol != '#') {
                                    if (first[A].find(symbol) == first[A].end()) {
                                        first[A].insert(symbol);
                                        changed = true;
                                    }
                                }
                            }
                            
                            if (first[X].find('#') == first[X].end()) {
                                canDeriveEpsilon = false;
                                break;
                            }
                        }
                        k++;
                    }
                    
                    if (canDeriveEpsilon && k == alpha.length()) {
                        if (first[A].find('#') == first[A].end()) {
                            first[A].insert('#');
                            changed = true;
                        }
                    }
                }
            }
        }
    }
    
    void calculateFollow() {
        cout << "\nCalculating FOLLOW sets..." << endl;
        
        for (char nt : non_terminals) {
            follow[nt] = set<char>();
        }
        
        follow[start_symbol].insert('$');
        
        bool changed = true;
        while (changed) {
            changed = false;
            
            for (const Production& prod : productions) {
                char A = prod.lhs;
                string beta = prod.rhs;
                
                for (int i = 0; i < beta.length(); i++) {
                    char B = beta[i];
                    
                    if (B >= 'A' && B <= 'Z') {
                        string gamma = beta.substr(i + 1);
                        
                        if (gamma.empty()) {
                            for (char symbol : follow[A]) {
                                if (follow[B].find(symbol) == follow[B].end()) {
                                    follow[B].insert(symbol);
                                    changed = true;
                                }
                            }
                        } else {
                            bool allDeriveEpsilon = true;
                            
                            for (char X : gamma) {
                                if (X < 'A' || X > 'Z') {
                                    if (follow[B].find(X) == follow[B].end()) {
                                        follow[B].insert(X);
                                        changed = true;
                                    }
                                    allDeriveEpsilon = false;
                                    break;
                                } else {
                                    for (char symbol : first[X]) {
                                        if (symbol != '#') {
                                            if (follow[B].find(symbol) == follow[B].end()) {
                                                follow[B].insert(symbol);
                                                changed = true;
                                            }
                                        }
                                    }
                                    
                                    if (first[X].find('#') == first[X].end()) {
                                        allDeriveEpsilon = false;
                                        break;
                                    }
                                }
                            }
                            
                            if (allDeriveEpsilon) {
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
    }
    
    void printGrammar() {
        cout << "\nGrammar Productions:" << endl;
        for (const Production& prod : productions) {
            cout << prod.lhs << " -> " << prod.rhs << endl;
        }
        cout << "Start Symbol: " << start_symbol << endl;
    }
    
    void printFirst() {
        cout << "\nFIRST Sets:" << endl;
        cout << "--------------------" << endl;
        for (char nt : non_terminals) {
            cout << "FIRST(" << nt << ") = {";
            bool first_item = true;
            for (char symbol : first[nt]) {
                if (!first_item) cout << ", ";
                cout << symbol;
                first_item = false;
            }
            cout << "}" << endl;
        }
    }
    
    void printFollow() {
        cout << "\nFOLLOW Sets:" << endl;
        cout << "--------------------" << endl;
        for (char nt : non_terminals) {
            cout << "FOLLOW(" << nt << ") = {";
            bool first_item = true;
            for (char symbol : follow[nt]) {
                if (!first_item) cout << ", ";
                cout << symbol;
                first_item = false;
            }
            cout << "}" << endl;
        }
    }
};

int main() {
    FirstFollowCalculator calc;
    string production;
    
    cout << "FIRST and FOLLOW Set Calculator" << endl;
    cout << "===============================" << endl;
    cout << "Enter grammar productions (Enter 'done' to finish)" << endl;
    cout << "Format: A->BC or A->a or A-># (for epsilon)" << endl;
    cout << "Use '#' for epsilon productions" << endl;
    cout << "First production's LHS will be the start symbol" << endl;
    cout << endl;
    
    while (true) {
        cout << "Enter production: ";
        getline(cin, production);
        
        if (production == "done") {
            break;
        }
        
        if (production.find("->") != string::npos) {
            char lhs = production[0];
            string rhs = production.substr(production.find("->") + 2);
            calc.addProduction(lhs, rhs);
            cout << "Added: " << lhs << " -> " << rhs << endl;
        } else {
            cout << "Invalid format! Use A->BC" << endl;
        }
    }
    
    calc.printGrammar();
    calc.calculateFirst();
    calc.printFirst();
    calc.calculateFollow();
    calc.printFollow();
    
    return 0;
}
