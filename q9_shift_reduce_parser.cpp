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
    cout << "Enter grammar productions (Enter 'done' to finish)" << endl;
    cout << "Format: A->BC or A->a" << endl;
    cout << "First production's LHS will be the start symbol" << endl << endl;
    
    ShiftReduceParser parser;
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
