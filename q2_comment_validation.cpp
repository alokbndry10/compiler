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
    vector<string> test_cases = {
        "// This is a single line comment",
        "/* This is a multi-line comment */",
        "/* Multi-line\n   comment\n   example */",
        "// Another single line",
        "/* Invalid comment",
        "Invalid // comment",
        "/ Not a comment",
        "/* */",
        "//"
    };
    
    cout << "Comment Validation Program" << endl;
    cout << "==========================" << endl;
    
    for (const string& test : test_cases) {
        cout << "\nInput: \"" << test << "\"" << endl;
        
        if (is_single_line_comment(test)) {
            cout << "Type: Single-line comment" << endl;
            cout << "Valid: YES" << endl;
        } else if (is_multi_line_comment(test)) {
            cout << "Type: Multi-line comment" << endl;
            cout << "Valid: YES" << endl;
        } else {
            cout << "Type: Not a comment" << endl;
            cout << "Valid: NO" << endl;
        }
        cout << "------------------------" << endl;
    }
    
    return 0;
}
