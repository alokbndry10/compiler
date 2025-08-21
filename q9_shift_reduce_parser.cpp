// Question 9: Write a program to implement Shift Reduce Parser

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
using namespace std;

struct Production {
    char lhs;
    string rhs;
    
    Production(char l, string r) : lhs(l), rhs(r) {}
    
    bool operator==(const Production& other) const {
        return lhs == other.lhs && rhs == other.rhs;
    }
};

class ShiftReduceParser {
private:
    vector<Production> grammar;
    stack<char> parseStack;
    string input;
    size_t inputPointer;
    vector<string> actions;
    
public:
    void addProduction(char lhs, string rhs) {
        grammar.push_back(Production(lhs, rhs));
    }
    
    void printGrammar() {
        cout << "Grammar:" << endl;
        for (size_t i = 0; i < grammar.size(); i++) {
            cout << i + 1 << ". " << grammar[i].lhs << " -> " << grammar[i].rhs << endl;
        }
        cout << endl;
    }
    
    string stackToString() {
        stack<char> temp = parseStack;
        string result = "";
        vector<char> chars;
        
        while (!temp.empty()) {
            chars.push_back(temp.top());
            temp.pop();
        }
        
        reverse(chars.begin(), chars.end());
        for (char c : chars) {
            result += c;
        }
        
        return result.empty() ? "$" : result;
    }
    
    string remainingInput() {
        if (inputPointer >= input.length()) {
            return "$";
        }
        return input.substr(inputPointer) + "$";
    }
    
    bool canReduce(int& prodIndex) {
        for (size_t i = 0; i < grammar.size(); i++) {
            string rhs = grammar[i].rhs;
            
            // Check if top of stack matches RHS of production
            if (parseStack.size() >= rhs.length()) {
                stack<char> temp = parseStack;
                string topSymbols = "";
                
                // Extract top symbols equal to RHS length
                for (size_t j = 0; j < rhs.length(); j++) {
                    if (!temp.empty()) {
                        topSymbols = temp.top() + topSymbols;
                        temp.pop();
                    }
                }
                
                if (topSymbols == rhs) {
                    prodIndex = i;
                    return true;
                }
            }
        }
        return false;
    }
    
    void reduce(int prodIndex) {
        Production prod = grammar[prodIndex];
        
        // Pop RHS symbols from stack
        for (size_t i = 0; i < prod.rhs.length(); i++) {
            if (!parseStack.empty()) {
                parseStack.pop();
            }
        }
        
        // Push LHS
        parseStack.push(prod.lhs);
        
        actions.push_back("Reduce by " + to_string(prodIndex + 1) + ": " + 
                         prod.lhs + " -> " + prod.rhs);
    }
    
    void shift() {
        if (inputPointer < input.length()) {
            parseStack.push(input[inputPointer]);
            actions.push_back("Shift " + string(1, input[inputPointer]));
            inputPointer++;
        }
    }
    
    void printStep(int step, string action) {
        cout << step << "\t" << stackToString() << "\t\t" 
             << remainingInput() << "\t\t" << action << endl;
    }
    
    bool parse(string inputString) {
        input = inputString;
        inputPointer = 0;
        actions.clear();
        
        // Clear stack
        while (!parseStack.empty()) {
            parseStack.pop();
        }
        
        cout << "Parsing string: " << input << endl;
        cout << string(50, '-') << endl;
        cout << "Step\tStack\t\tInput\t\tAction" << endl;
        cout << string(50, '-') << endl;
        
        int step = 1;
        printStep(step++, "Initialize");
        
        while (true) {
            int prodIndex;
            
            // Try to reduce first
            if (canReduce(prodIndex)) {
                reduce(prodIndex);
                printStep(step++, actions.back());
                
                // Check if we've reduced to start symbol and input is consumed
                if (parseStack.size() == 1 && parseStack.top() == grammar[0].lhs && 
                    inputPointer >= input.length()) {
                    cout << string(50, '-') << endl;
                    cout << "SUCCESS: String accepted!" << endl;
                    return true;
                }
            }
            // If can't reduce, try to shift
            else if (inputPointer < input.length()) {
                shift();
                printStep(step++, actions.back());
            }
            // If can't shift or reduce, parsing fails
            else {
                cout << string(50, '-') << endl;
                cout << "ERROR: String rejected!" << endl;
                return false;
            }
            
            // Prevent infinite loops
            if (step > 50) {
                cout << "ERROR: Too many steps, possible infinite loop!" << endl;
                return false;
            }
        }
    }
};

int main() {
    cout << "Shift-Reduce Parser Implementation" << endl;
    cout << "==================================" << endl;
    
    // Example 1: Simple arithmetic grammar
    cout << "Example 1: Simple Arithmetic Grammar" << endl;
    cout << "------------------------------------" << endl;
    
    ShiftReduceParser parser1;
    parser1.addProduction('E', "E+T");
    parser1.addProduction('E', "T");
    parser1.addProduction('T', "T*F");
    parser1.addProduction('T', "F");
    parser1.addProduction('F', "(E)");
    parser1.addProduction('F', "id");
    
    parser1.printGrammar();
    
    // Test cases
    vector<string> testCases1 = {"id", "id+id", "id*id", "(id)", "id+id*id"};
    
    for (const string& test : testCases1) {
        cout << "\n";
        parser1.parse(test);
        cout << "\n" << string(60, '=') << "\n";
    }
    
    // Example 2: Simpler grammar
    cout << "\nExample 2: Simpler Grammar" << endl;
    cout << "--------------------------" << endl;
    
    ShiftReduceParser parser2;
    parser2.addProduction('S', "aA");
    parser2.addProduction('A', "b");
    
    parser2.printGrammar();
    
    vector<string> testCases2 = {"ab", "aab", "ba", "a", "b"};
    
    for (const string& test : testCases2) {
        cout << "\n";
        parser2.parse(test);
        cout << "\n" << string(60, '=') << "\n";
    }
    
    return 0;
}
