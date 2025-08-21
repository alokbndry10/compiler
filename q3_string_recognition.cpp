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
    vector<string> test_strings = {
        "", "a", "aa", "aaa", "b", "bb", "ab", "aab", "abb", "abbb", "aabb", "abab", "baa", "abc"
    };
    
    cout << "String Pattern Recognition Program" << endl;
    cout << "==================================" << endl;
    
    for (const string& str : test_strings) {
        cout << "\nTesting string: \"" << str << "\"" << endl;
        
        cout << "Matches a*: " << (matches_a_star(str) ? "YES" : "NO") << endl;
        cout << "Matches a*b*: " << (matches_a_star_b_star(str) ? "YES" : "NO") << endl;
        cout << "Matches abb: " << (matches_abb(str) ? "YES" : "NO") << endl;
        
        cout << "------------------------" << endl;
    }
    
    // Interactive mode
    string input;
    cout << "\nEnter a string to test (or 'quit' to exit): ";
    while (cin >> input && input != "quit") {
        cout << "\nTesting string: \"" << input << "\"" << endl;
        cout << "Matches a*: " << (matches_a_star(input) ? "YES" : "NO") << endl;
        cout << "Matches a*b*: " << (matches_a_star_b_star(input) ? "YES" : "NO") << endl;
        cout << "Matches abb: " << (matches_abb(input) ? "YES" : "NO") << endl;
        cout << "\nEnter another string (or 'quit' to exit): ";
    }
    
    return 0;
}
