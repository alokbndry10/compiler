// Question 1: Write a program to implement DFA that accepts string ending with 'abb'

#include <iostream>
#include <string>
using namespace std;

bool dfa_ends_with_abb(const string& s) {
    int state = 0;
    
    cout << "Processing string: " << s << endl;
    cout << "State transitions:" << endl;
    
    for (int i = 0; i < s.length(); i++) {
        char c = s[i];
        int prev_state = state;
        
        switch (state) {
            case 0: // Initial state q0
                state = (c == 'a') ? 1 : 0;
                break;
            case 1: // State q1 (after 'a')
                state = (c == 'a') ? 1 : (c == 'b') ? 2 : 0;
                break;
            case 2: // State q2 (after 'ab')
                state = (c == 'b') ? 3 : (c == 'a') ? 1 : 0;
                break;
            case 3: // State q3 (after 'abb' - accepting state)
                state = (c == 'a') ? 1 : 0;
                break;
        }
        
        cout << "Input: " << c << ", State: q" << prev_state << " -> q" << state << endl;
    }
    
    return state == 3;
}

int main() {
    string input;
    char choice;
    
    cout << "DFA that accepts strings ending with 'abb'" << endl;
    cout << "===========================================" << endl;
    cout << "States: q0(start), q1(after 'a'), q2(after 'ab'), q3(after 'abb' - final)" << endl;
    cout << "Alphabet: {a, b}" << endl << endl;
    
    do {
        cout << "Enter a string to test: ";
        cin >> input;
        
        cout << "\nTesting string: " << input << endl;
        bool accepted = dfa_ends_with_abb(input);
        cout << "Final State: q" << (accepted ? 3 : -1) << endl;
        cout << "Result: " << (accepted ? "ACCEPTED" : "REJECTED") << endl;
        
        cout << "\nDo you want to test another string? (y/n): ";
        cin >> choice;
        cout << endl;
        
    } while (choice == 'y' || choice == 'Y');
    
    return 0;
}
