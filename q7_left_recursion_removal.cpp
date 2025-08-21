// Question 7: Write a program to remove left recursion from any grammar

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

struct Production {
    char lhs;
    string rhs;
    
    Production(char l, string r) : lhs(l), rhs(r) {}
};

class LeftRecursionRemover {
private:
    vector<Production> productions;
    map<char, vector<string>> grammar;
    char newNonTerminal;
    
public:
    LeftRecursionRemover() : newNonTerminal('A') {}
    
    void addProduction(char lhs, string rhs) {
        productions.push_back(Production(lhs, rhs));
        grammar[lhs].push_back(rhs);
    }
    
    void printOriginalGrammar() {
        cout << "Original Grammar:" << endl;
        for (const auto& rule : grammar) {
            cout << rule.first << " -> ";
            for (size_t i = 0; i < rule.second.size(); i++) {
                if (i > 0) cout << " | ";
                cout << rule.second[i];
            }
            cout << endl;
        }
        cout << endl;
    }
    
    char getNewNonTerminal() {
        while (grammar.find(newNonTerminal) != grammar.end()) {
            newNonTerminal++;
        }
        return newNonTerminal++;
    }
    
    void removeDirectLeftRecursion(char A) {
        vector<string> alpha; // Left recursive productions (A -> Aα)
        vector<string> beta;  // Non-left recursive productions (A -> β)
        
        // Separate left recursive and non-left recursive productions
        for (const string& production : grammar[A]) {
            if (!production.empty() && production[0] == A) {
                // Left recursive: A -> Aα
                alpha.push_back(production.substr(1)); // Extract α
            } else {
                // Non-left recursive: A -> β
                beta.push_back(production);
            }
        }
        
        if (alpha.empty()) {
            cout << "No direct left recursion found for " << A << endl;
            return;
        }
        
        cout << "Removing direct left recursion for " << A << ":" << endl;
        cout << "Left recursive parts (α): ";
        for (size_t i = 0; i < alpha.size(); i++) {
            if (i > 0) cout << ", ";
            cout << alpha[i];
        }
        cout << endl;
        
        cout << "Non-left recursive parts (β): ";
        for (size_t i = 0; i < beta.size(); i++) {
            if (i > 0) cout << ", ";
            cout << beta[i];
        }
        cout << endl;
        
        // Get new non-terminal
        char A_prime = getNewNonTerminal();
        
        // Clear old productions for A
        grammar[A].clear();
        
        // Add new productions: A -> βA'
        for (const string& b : beta) {
            grammar[A].push_back(b + A_prime);
        }
        
        // Add new productions: A' -> αA' | ε
        for (const string& a : alpha) {
            grammar[A_prime].push_back(a + A_prime);
        }
        grammar[A_prime].push_back("ε");
        
        cout << "New non-terminal introduced: " << A_prime << endl;
        cout << endl;
    }
    
    void removeIndirectLeftRecursion() {
        cout << "Checking for indirect left recursion..." << endl;
        
        vector<char> nonTerminals;
        for (const auto& rule : grammar) {
            nonTerminals.push_back(rule.first);
        }
        
        // Sort non-terminals to process in order
        sort(nonTerminals.begin(), nonTerminals.end());
        
        for (size_t i = 0; i < nonTerminals.size(); i++) {
            char Ai = nonTerminals[i];
            
            // For each Aj where j < i
            for (size_t j = 0; j < i; j++) {
                char Aj = nonTerminals[j];
                
                vector<string> newProductions;
                bool changed = false;
                
                // For each production Ai -> Aj γ
                for (const string& production : grammar[Ai]) {
                    if (!production.empty() && production[0] == Aj) {
                        changed = true;
                        string gamma = production.substr(1);
                        
                        // Replace Ai -> Aj γ with Ai -> δ γ for each Aj -> δ
                        for (const string& delta : grammar[Aj]) {
                            newProductions.push_back(delta + gamma);
                        }
                    } else {
                        newProductions.push_back(production);
                    }
                }
                
                if (changed) {
                    cout << "Substituting " << Aj << " in productions of " << Ai << endl;
                    grammar[Ai] = newProductions;
                }
            }
            
            // Remove direct left recursion for Ai
            removeDirectLeftRecursion(Ai);
        }
    }
    
    void printModifiedGrammar() {
        cout << "Grammar after removing left recursion:" << endl;
        for (const auto& rule : grammar) {
            cout << rule.first << " -> ";
            for (size_t i = 0; i < rule.second.size(); i++) {
                if (i > 0) cout << " | ";
                cout << rule.second[i];
            }
            cout << endl;
        }
        cout << endl;
    }
    
    void removeLeftRecursion() {
        cout << "Starting left recursion removal process..." << endl;
        cout << string(50, '=') << endl;
        
        printOriginalGrammar();
        removeIndirectLeftRecursion();
        printModifiedGrammar();
    }
};

int main() {
    cout << "Left Recursion Removal Program" << endl;
    cout << "==============================" << endl;
    
    // Example 1: Simple direct left recursion
    cout << "Example 1: Direct Left Recursion" << endl;
    cout << "---------------------------------" << endl;
    LeftRecursionRemover remover1;
    remover1.addProduction('A', "Aa");
    remover1.addProduction('A', "b");
    remover1.removeLeftRecursion();
    
    cout << string(60, '=') << endl;
    
    // Example 2: Multiple alternatives with left recursion
    cout << "Example 2: Multiple Alternatives" << endl;
    cout << "--------------------------------" << endl;
    LeftRecursionRemover remover2;
    remover2.addProduction('E', "E+T");
    remover2.addProduction('E', "E-T");
    remover2.addProduction('E', "T");
    remover2.addProduction('T', "T*F");
    remover2.addProduction('T', "F");
    remover2.addProduction('F', "(E)");
    remover2.addProduction('F', "id");
    remover2.removeLeftRecursion();
    
    cout << string(60, '=') << endl;
    
    // Example 3: Indirect left recursion
    cout << "Example 3: Indirect Left Recursion" << endl;
    cout << "-----------------------------------" << endl;
    LeftRecursionRemover remover3;
    remover3.addProduction('A', "Ba");
    remover3.addProduction('A', "c");
    remover3.addProduction('B', "Ab");
    remover3.addProduction('B', "d");
    remover3.removeLeftRecursion();
    
    return 0;
}
