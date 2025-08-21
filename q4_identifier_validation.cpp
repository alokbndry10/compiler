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
    vector<string> test_identifiers = {
        "variable",
        "_variable",
        "var123",
        "_123var",
        "123var",
        "var_name",
        "VarName",
        "var-name",
        "var name",
        "var$",
        "_",
        "a",
        "",
        "class",
        "myVariable_123"
    };
    
    cout << "Identifier Validation Program" << endl;
    cout << "=============================" << endl;
    
    for (const string& identifier : test_identifiers) {
        cout << "\n";
        analyze_identifier(identifier);
        cout << "------------------------" << endl;
    }
    
    // Interactive mode
    string input;
    cout << "\nEnter an identifier to validate (or 'quit' to exit): ";
    while (getline(cin, input) && input != "quit") {
        if (!input.empty()) {
            cout << "\n";
            analyze_identifier(input);
            cout << "------------------------" << endl;
        }
        cout << "\nEnter another identifier (or 'quit' to exit): ";
    }
    
    return 0;
}
