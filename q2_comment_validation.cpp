// Question 2: Write a program for comment validation

#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool is_single_line_comment(const string& s) {
    return s.length() >= 2 && s.substr(0, 2) == "//";
}

bool is_multi_line_comment(const string& s) {
    return s.length() >= 4 && s.substr(0, 2) == "/*" && s.substr(s.length() - 2) == "*/";
}

bool is_valid_comment(const string& s) {
    return is_single_line_comment(s) || is_multi_line_comment(s);
}

int main() {
    string input;
    char choice;
    
    cout << "Comment Validation Program" << endl;
    cout << "==========================" << endl;
    cout << "Supports C/C++ style comments:" << endl;
    cout << "1. Single-line: // comment" << endl;
    cout << "2. Multi-line: /* comment */" << endl << endl;
    
    do {
        cout << "Enter a string to validate: ";
        getline(cin, input);
        
        cout << "\nInput: \"" << input << "\"" << endl;
        
        if (is_single_line_comment(input)) {
            cout << "Type: Single-line comment" << endl;
            cout << "Valid: YES" << endl;
        } else if (is_multi_line_comment(input)) {
            cout << "Type: Multi-line comment" << endl;
            cout << "Valid: YES" << endl;
        } else {
            cout << "Type: Not a valid comment" << endl;
            cout << "Valid: NO" << endl;
        }
        
        cout << "\nDo you want to test another string? (y/n): ";
        cin >> choice;
        cin.ignore(); // Clear input buffer
        cout << endl;
        
    } while (choice == 'y' || choice == 'Y');
    
    return 0;
}
