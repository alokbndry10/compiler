// Question 1: Write a program to implement DFA that accepts string ending with 'abb'

#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool dfa_ends_with_abb(const string& s) {
    int state = 0;
    
    cout << "Processing string: " << s << endl;
    cout << "State transitions:" << endl;
    
    for (int i = 0; i < s.length(); i++) {
        char c = s[i];
        int prev_state = state;
        
        switch (state) {
            case 0: // Initial state
                state = (c == 'a') ? 1 : 0;
                break;
            case 1: // After 'a'
                state = (c == 'a') ? 1 : (c == 'b') ? 2 : 0;
                break;
            case 2: // After 'ab'
                state = (c == 'b') ? 3 : (c == 'a') ? 1 : 0;
                break;
            case 3: // After 'abb' (accepting state)
                state = (c == 'a') ? 1 : 0;
                break;
        }
        
        cout << "Input: " << c << ", State: " << prev_state << " -> " << state << endl;
    }
    
    return state == 3;
}

int main() {
    vector<string> test_strings = {"abb", "aabb", "ababb", "abba", "abc", "baabb"};
    
    cout << "DFA that accepts strings ending with 'abb'" << endl;
    cout << "===========================================" << endl;
    
    for (const string& str : test_strings) {
        cout << "\nTesting: " << str << endl;
        bool accepted = dfa_ends_with_abb(str);
        cout << "Result: " << (accepted ? "ACCEPTED" : "REJECTED") << endl;
        cout << "------------------------" << endl;
    }
    
    return 0;
}
