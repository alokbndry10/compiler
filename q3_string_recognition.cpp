// Question 3: Write a program to recognize string under 'a*', 'a*b*', 'abb'

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

bool matches_a_star(const string& s) {
    // a* means zero or more 'a's
    return all_of(s.begin(), s.end(), [](char c) { return c == 'a'; });
}

bool matches_a_star_b_star(const string& s) {
    // a*b* means zero or more 'a's followed by zero or more 'b's
    size_t i = 0;
    
    // Skip all 'a's
    while (i < s.size() && s[i] == 'a') {
        i++;
    }
    
    // Skip all 'b's
    while (i < s.size() && s[i] == 'b') {
        i++;
    }
    
    // If we've processed the entire string, it matches a*b*
    return i == s.size();
}

bool matches_abb(const string& s) {
    // Exact match for "abb"
    return s == "abb";
}

int main() {
    string input;
    char choice;
    
    cout << "String Pattern Recognition Program" << endl;
    cout << "==================================" << endl;
    cout << "Patterns:" << endl;
    cout << "1. a* - Zero or more 'a's" << endl;
    cout << "2. a*b* - Zero or more 'a's followed by zero or more 'b's" << endl;
    cout << "3. abb - Exact string 'abb'" << endl << endl;
    
    do {
        cout << "Enter a string to test: ";
        cin >> input;
        
        cout << "\nTesting string: \"" << input << "\"" << endl;
        cout << "Matches a*: " << (matches_a_star(input) ? "YES" : "NO") << endl;
        cout << "Matches a*b*: " << (matches_a_star_b_star(input) ? "YES" : "NO") << endl;
        cout << "Matches abb: " << (matches_abb(input) ? "YES" : "NO") << endl;
        
        cout << "\nDo you want to test another string? (y/n): ";
        cin >> choice;
        cout << endl;
        
    } while (choice == 'y' || choice == 'Y');
    
    return 0;
}
