// Question 11: Write a program for Intermediate Code Generation

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
using namespace std;

struct ThreeAddressCode {
    string op;      // operator
    string arg1;    // first operand
    string arg2;    // second operand (empty for unary)
    string result;  // result variable
    
    ThreeAddressCode(string operation, string operand1, string operand2, string res)
        : op(operation), arg1(operand1), arg2(operand2), result(res) {}
    
    string toString() const {
        if (arg2.empty()) {
            return result + " = " + op + " " + arg1;
        } else {
            return result + " = " + arg1 + " " + op + " " + arg2;
        }
    }
};

class IntermediateCodeGenerator {
private:
    vector<ThreeAddressCode> code;
    int tempCount;
    
public:
    IntermediateCodeGenerator() : tempCount(1) {}
    
    string newTemp() {
        return "t" + to_string(tempCount++);
    }
    
    void generateCode(string op, string arg1, string arg2, string result) {
        code.push_back(ThreeAddressCode(op, arg1, arg2, result));
    }
    
    void generateCode(string op, string arg1, string result) {
        code.push_back(ThreeAddressCode(op, arg1, "", result));
    }
    
    void printCode() {
        cout << "Three Address Code:" << endl;
        cout << string(30, '-') << endl;
        for (size_t i = 0; i < code.size(); i++) {
            cout << i + 1 << ". " << code[i].toString() << endl;
        }
        cout << endl;
    }
    
    void clear() {
        code.clear();
        tempCount = 1;
    }
    
    // Generate code for arithmetic expression: a = b + c * d
    void generateArithmeticExpression() {
        cout << "Expression: a = b + c * d" << endl;
        cout << "Operator precedence: * has higher precedence than +" << endl;
        cout << endl;
        
        string t1 = newTemp();
        generateCode("*", "c", "d", t1);
        
        string t2 = newTemp();
        generateCode("+", "b", t1, t2);
        
        generateCode("=", t2, "a");
        
        printCode();
    }
    
    // Generate code for array access: a[i] = b[j] + c
    void generateArrayAccess() {
        cout << "Expression: a[i] = b[j] + c" << endl;
        cout << "Array access with index calculation" << endl;
        cout << endl;
        
        // Calculate address for b[j]
        string t1 = newTemp();
        generateCode("*", "j", "4", t1);  // Assuming 4 bytes per element
        
        string t2 = newTemp();
        generateCode("+", "b", t1, t2);
        
        string t3 = newTemp();
        generateCode("=", "*" + t2, t3);  // Load b[j]
        
        // Add c to b[j]
        string t4 = newTemp();
        generateCode("+", t3, "c", t4);
        
        // Calculate address for a[i]
        string t5 = newTemp();
        generateCode("*", "i", "4", t5);
        
        string t6 = newTemp();
        generateCode("+", "a", t5, t6);
        
        // Store result in a[i]
        generateCode("=", t4, "*" + t6);
        
        printCode();
    }
    
    // Generate code for conditional statement: if (a > b) x = y; else x = z;
    void generateConditional() {
        cout << "Statement: if (a > b) x = y; else x = z;" << endl;
        cout << "Conditional jump implementation" << endl;
        cout << endl;
        
        string L1 = "L1";  // else label
        string L2 = "L2";  // end label
        
        // Compare a and b
        generateCode("if", "a", "<=", "b");
        code.push_back(ThreeAddressCode("goto", L1, "", ""));
        
        // True branch: x = y
        generateCode("=", "y", "x");
        code.push_back(ThreeAddressCode("goto", L2, "", ""));
        
        // False branch: x = z
        code.push_back(ThreeAddressCode(L1 + ":", "", "", ""));
        generateCode("=", "z", "x");
        
        // End label
        code.push_back(ThreeAddressCode(L2 + ":", "", "", ""));
        
        printCode();
    }
    
    // Generate code for while loop: while (i < n) { sum = sum + a[i]; i = i + 1; }
    void generateWhileLoop() {
        cout << "Statement: while (i < n) { sum = sum + a[i]; i = i + 1; }" << endl;
        cout << "Loop with array access" << endl;
        cout << endl;
        
        string L1 = "L1";  // loop start
        string L2 = "L2";  // loop end
        
        // Loop start label
        code.push_back(ThreeAddressCode(L1 + ":", "", "", ""));
        
        // Check condition i < n
        generateCode("if", "i", ">=", "n");
        code.push_back(ThreeAddressCode("goto", L2, "", ""));
        
        // Loop body: sum = sum + a[i]
        string t1 = newTemp();
        generateCode("*", "i", "4", t1);  // Calculate offset
        
        string t2 = newTemp();
        generateCode("+", "a", t1, t2);   // Calculate address
        
        string t3 = newTemp();
        generateCode("=", "*" + t2, t3);  // Load a[i]
        
        string t4 = newTemp();
        generateCode("+", "sum", t3, t4); // sum + a[i]
        
        generateCode("=", t4, "sum");     // sum = sum + a[i]
        
        // Increment i
        string t5 = newTemp();
        generateCode("+", "i", "1", t5);
        generateCode("=", t5, "i");
        
        // Jump back to loop start
        code.push_back(ThreeAddressCode("goto", L1, "", ""));
        
        // Loop end label
        code.push_back(ThreeAddressCode(L2 + ":", "", "", ""));
        
        printCode();
    }
    
    // Generate code for function call: result = func(a, b, c)
    void generateFunctionCall() {
        cout << "Statement: result = func(a, b, c)" << endl;
        cout << "Function call with parameters" << endl;
        cout << endl;
        
        // Push parameters (reverse order for stack)
        code.push_back(ThreeAddressCode("param", "c", "", ""));
        code.push_back(ThreeAddressCode("param", "b", "", ""));
        code.push_back(ThreeAddressCode("param", "a", "", ""));
        
        // Call function
        string t1 = newTemp();
        code.push_back(ThreeAddressCode("call", "func", "3", t1));  // 3 parameters
        
        // Store return value
        generateCode("=", t1, "result");
        
        printCode();
    }
};

int main() {
    cout << "Intermediate Code Generator" << endl;
    cout << "===========================" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Arithmetic Expression (a = b + c * d)" << endl;
    cout << "2. Array Access (a[i] = b[j] + c)" << endl;
    cout << "3. Conditional Statement (if-else)" << endl;
    cout << "4. While Loop" << endl;
    cout << "5. Function Call" << endl;
    cout << "6. Custom Expression" << endl;
    
    IntermediateCodeGenerator generator;
    int choice;
    
    while (true) {
        cout << "\nEnter choice (1-6, 0 to exit): ";
        cin >> choice;
        
        switch (choice) {
            case 0:
                cout << "Exiting..." << endl;
                return 0;
                
            case 1:
                cout << "\nGenerating code for: a = b + c * d" << endl;
                generator.generateArithmeticExpression();
                break;
                
            case 2:
                cout << "\nGenerating code for: a[i] = b[j] + c" << endl;
                generator.generateArrayAccess();
                break;
                
            case 3:
                cout << "\nGenerating code for: if (a > b) x = y; else x = z;" << endl;
                generator.generateConditional();
                break;
                
            case 4:
                cout << "\nGenerating code for: while (i < n) { sum = sum + a[i]; i = i + 1; }" << endl;
                generator.generateWhileLoop();
                break;
                
            case 5:
                cout << "\nGenerating code for: result = func(a, b, c)" << endl;
                generator.generateFunctionCall();
                break;
                
            case 6:
                cout << "\nCustom expression generator not implemented in this version." << endl;
                cout << "Use options 1-5 for predefined expressions." << endl;
                break;
                
            default:
                cout << "Invalid choice! Please enter 1-6 or 0 to exit." << endl;
                continue;
        }
        
        generator.clear();
    }
    
    return 0;
}
