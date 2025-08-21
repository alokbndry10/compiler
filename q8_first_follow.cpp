// Question 8: Write a program to find out FIRST and FOLLOW of grammar

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
    set<char> terminals;
    set<char> non_terminals;
    char start_symbol;
    
public:
    FirstFollowCalculator(char start = 'S') : start_symbol(start) {}
    
    void addProduction(char lhs, string rhs) {
        productions.push_back(Production(lhs, rhs));
        non_terminals.insert(lhs);
        
        if (productions.size() == 1) {
            start_symbol = lhs; // First production's LHS is start symbol
        }
        
        for (char c : rhs) {
            if (c >= 'A' && c <= 'Z') {
                non_terminals.insert(c);
            } else if (c != 'ε') {
                terminals.insert(c);
            }
        }
    }
    
    void calculateFirst() {
        cout << "Calculating FIRST sets..." << endl;
        
        // Initialize FIRST sets
        for (char nt : non_terminals) {
            first[nt] = set<char>();
        }
        
        bool changed = true;
        while (changed) {
            changed = false;
            
            for (const Production& prod : productions) {
                char A = prod.lhs;
                string alpha = prod.rhs;
                
                cout << "Processing: " << A << " -> " << alpha << endl;
                
                if (alpha == "ε") {
                    if (first[A].find('ε') == first[A].end()) {
                        first[A].insert('ε');
                        changed = true;
                        cout << "  Added ε to FIRST(" << A << ")" << endl;
                    }
                } else {
                    bool all_derive_epsilon = true;
                    
                    for (size_t i = 0; i < alpha.length() && all_derive_epsilon; i++) {
                        char X = alpha[i];
                        
                        if (terminals.find(X) != terminals.end()) {
                            // X is terminal
                            if (first[A].find(X) == first[A].end()) {
                                first[A].insert(X);
                                changed = true;
                                cout << "  Added " << X << " to FIRST(" << A << ")" << endl;
                            }
                            all_derive_epsilon = false;
                        } else {
                            // X is non-terminal
                            set<char> first_X = first[X];
                            
                            for (char symbol : first_X) {
                                if (symbol != 'ε' && first[A].find(symbol) == first[A].end()) {
                                    first[A].insert(symbol);
                                    changed = true;
                                    cout << "  Added " << symbol << " to FIRST(" << A << ") from FIRST(" << X << ")" << endl;
                                }
                            }
                            
                            if (first_X.find('ε') == first_X.end()) {
                                all_derive_epsilon = false;
                            }
                        }
                    }
                    
                    if (all_derive_epsilon && first[A].find('ε') == first[A].end()) {
                        first[A].insert('ε');
                        changed = true;
                        cout << "  Added ε to FIRST(" << A << ") - all symbols derive ε" << endl;
                    }
                }
                cout << endl;
            }
        }
    }
    
    void calculateFollow() {
        cout << "Calculating FOLLOW sets..." << endl;
        
        // Initialize FOLLOW sets
        for (char nt : non_terminals) {
            follow[nt] = set<char>();
        }
        
        // Add $ to FOLLOW of start symbol
        follow[start_symbol].insert('$');
        cout << "Added $ to FOLLOW(" << start_symbol << ") - start symbol" << endl;
        
        bool changed = true;
        while (changed) {
            changed = false;
            
            for (const Production& prod : productions) {
                char A = prod.lhs;
                string beta = prod.rhs;
                
                cout << "Processing: " << A << " -> " << beta << endl;
                
                for (size_t i = 0; i < beta.length(); i++) {
                    char B = beta[i];
                    
                    if (non_terminals.find(B) != non_terminals.end()) {
                        // B is a non-terminal
                        string gamma = beta.substr(i + 1);
                        
                        // Calculate FIRST(γ)
                        set<char> first_gamma;
                        bool gamma_derives_epsilon = true;
                        
                        for (char symbol : gamma) {
                            if (terminals.find(symbol) != terminals.end()) {
                                first_gamma.insert(symbol);
                                gamma_derives_epsilon = false;
                                break;
                            } else {
                                for (char f : first[symbol]) {
                                    if (f != 'ε') {
                                        first_gamma.insert(f);
                                    }
                                }
                                if (first[symbol].find('ε') == first[symbol].end()) {
                                    gamma_derives_epsilon = false;
                                    break;
                                }
                            }
                        }
                        
                        // Add FIRST(γ) - {ε} to FOLLOW(B)
                        for (char symbol : first_gamma) {
                            if (follow[B].find(symbol) == follow[B].end()) {
                                follow[B].insert(symbol);
                                changed = true;
                                cout << "  Added " << symbol << " to FOLLOW(" << B << ") from FIRST(γ)" << endl;
                            }
                        }
                        
                        // If γ derives ε, add FOLLOW(A) to FOLLOW(B)
                        if (gamma_derives_epsilon || gamma.empty()) {
                            for (char symbol : follow[A]) {
                                if (follow[B].find(symbol) == follow[B].end()) {
                                    follow[B].insert(symbol);
                                    changed = true;
                                    cout << "  Added " << symbol << " to FOLLOW(" << B << ") from FOLLOW(" << A << ")" << endl;
                                }
                            }
                        }
                    }
                }
                cout << endl;
            }
        }
    }
    
    void printGrammar() {
        cout << "Grammar Productions:" << endl;
        for (const Production& prod : productions) {
            cout << prod.lhs << " -> " << prod.rhs << endl;
        }
        cout << "Start Symbol: " << start_symbol << endl;
        cout << endl;
    }
    
    void printFirst() {
        cout << "FIRST Sets:" << endl;
        cout << string(20, '-') << endl;
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
        cout << "FOLLOW Sets:" << endl;
        cout << string(20, '-') << endl;
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
};

int main() {
    cout << "FIRST and FOLLOW Set Calculator" << endl;
    cout << "===============================" << endl;
    
    // Example 1: Simple arithmetic grammar
    cout << "Example 1: Arithmetic Expression Grammar" << endl;
    cout << "----------------------------------------" << endl;
    FirstFollowCalculator calc1;
    calc1.addProduction('E', "TX");
    calc1.addProduction('X', "+TX");
    calc1.addProduction('X', "ε");
    calc1.addProduction('T', "FY");
    calc1.addProduction('Y', "*FY");
    calc1.addProduction('Y', "ε");
    calc1.addProduction('F', "(E)");
    calc1.addProduction('F', "id");
    
    calc1.printGrammar();
    calc1.calculateFirst();
    calc1.printFirst();
    calc1.calculateFollow();
    calc1.printFollow();
    
    cout << string(60, '=') << endl;
    
    // Example 2: Simple grammar with epsilon
    cout << "Example 2: Grammar with Epsilon Productions" << endl;
    cout << "-------------------------------------------" << endl;
    FirstFollowCalculator calc2;
    calc2.addProduction('S', "AB");
    calc2.addProduction('A', "a");
    calc2.addProduction('A', "ε");
    calc2.addProduction('B', "b");
    calc2.addProduction('B', "c");
    
    calc2.printGrammar();
    calc2.calculateFirst();
    calc2.printFirst();
    calc2.calculateFollow();
    calc2.printFollow();
    
    return 0;
}
