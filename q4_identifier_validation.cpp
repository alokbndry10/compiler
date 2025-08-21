// Question 4: Write a program to check whether a given identifier is valid or not

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

bool is_valid_identifier(const string& identifier) {
    // Check if string is empty
    if (identifier.empty()) {
        return false;
    }
    
    // First character must be letter or underscore
    if (!(isalpha(identifier[0]) || identifier[0] == '_')) {
        return false;
    }
    
    // Remaining characters must be letters, digits, or underscores
    for (size_t i = 1; i < identifier.length(); i++) {
        if (!(isalnum(identifier[i]) || identifier[i] == '_')) {
            return false;
        }
    }
    
    return true;
}

void analyze_identifier(const string& identifier) {
    cout << "Identifier: \"" << identifier << "\"" << endl;
    
    if (identifier.empty()) {
        cout << "Error: Empty identifier" << endl;
        cout << "Valid: NO" << endl;
        return;
    }
    
    cout << "First character: '" << identifier[0] << "' ";
    if (isalpha(identifier[0])) {
        cout << "(letter) - Valid" << endl;
    } else if (identifier[0] == '_') {
        cout << "(underscore) - Valid" << endl;
    } else if (isdigit(identifier[0])) {
        cout << "(digit) - Invalid! Identifiers cannot start with digit" << endl;
    } else {
        cout << "(special character) - Invalid!" << endl;
    }
    
    bool all_valid = true;
    for (size_t i = 1; i < identifier.length(); i++) {
        if (!(isalnum(identifier[i]) || identifier[i] == '_')) {
            cout << "Invalid character at position " << i << ": '" << identifier[i] << "'" << endl;
            all_valid = false;
        }
    }
    
    bool valid = is_valid_identifier(identifier);
    cout << "Overall validity: " << (valid ? "VALID" : "INVALID") << endl;
}

int main() {
    string identifier;
    char choice;
    
    cout << "Identifier Validation Program" << endl;
    cout << "=============================" << endl;
    cout << "Rules for valid identifiers:" << endl;
    cout << "1. Must start with a letter (a-z, A-Z) or underscore (_)" << endl;
    cout << "2. Can contain letters, digits (0-9), and underscores" << endl;
    cout << "3. Cannot be empty" << endl << endl;
    
    do {
        cout << "Enter an identifier to validate: ";
        cin >> identifier;
        
        cout << "\n";
        analyze_identifier(identifier);
        
        cout << "\nDo you want to test another identifier? (y/n): ";
        cin >> choice;
        cout << endl;
        
    } while (choice == 'y' || choice == 'Y');
    
    return 0;
}
