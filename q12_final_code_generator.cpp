// Question 12: Write a program for Final Code Generator

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

struct ThreeAddressCode {
    string op;
    string arg1;
    string arg2;
    string result;
    
    ThreeAddressCode(string operation, string operand1, string operand2, string res)
        : op(operation), arg1(operand1), arg2(operand2), result(res) {}
};

struct AssemblyInstruction {
    string opcode;
    string operand1;
    string operand2;
    
    AssemblyInstruction(string op, string op1 = "", string op2 = "")
        : opcode(op), operand1(op1), operand2(op2) {}
    
    string toString() const {
        if (operand1.empty()) {
            return opcode;
        } else if (operand2.empty()) {
            return opcode + " " + operand1;
        } else {
            return opcode + " " + operand1 + ", " + operand2;
        }
    }
};

class FinalCodeGenerator {
private:
    vector<ThreeAddressCode> intermediateCode;
    vector<AssemblyInstruction> assemblyCode;
    map<string, string> registerAllocation;
    set<string> availableRegisters;
    int currentRegister;
    
public:
    FinalCodeGenerator() : currentRegister(0) {
        // Initialize available registers
        availableRegisters.insert("R0");
        availableRegisters.insert("R1");
        availableRegisters.insert("R2");
        availableRegisters.insert("R3");
        availableRegisters.insert("R4");
        availableRegisters.insert("R5");
    }
    
    void addIntermediateCode(string op, string arg1, string arg2, string result) {
        intermediateCode.push_back(ThreeAddressCode(op, arg1, arg2, result));
    }
    
    string allocateRegister(string variable) {
        if (registerAllocation.find(variable) != registerAllocation.end()) {
            return registerAllocation[variable];
        }
        
        string reg = "R" + to_string(currentRegister);
        currentRegister = (currentRegister + 1) % 6;
        registerAllocation[variable] = reg;
        return reg;
    }
    
    string getOperand(string operand) {
        // Check if it's a temporary variable
        if (operand[0] == 't' && operand.length() > 1) {
            return allocateRegister(operand);
        }
        // Check if it's a variable
        else if ((operand[0] >= 'a' && operand[0] <= 'z') || 
                 (operand[0] >= 'A' && operand[0] <= 'Z')) {
            return operand;  // Memory location
        }
        // Check if it's a number
        else if (operand[0] >= '0' && operand[0] <= '9') {
            return "#" + operand;  // Immediate value
        }
        
        return operand;
    }
    
    void generateAssemblyCode() {
        cout << "Converting Three Address Code to Assembly Code" << endl;
        cout << string(50, '-') << endl;
        
        for (size_t i = 0; i < intermediateCode.size(); i++) {
            ThreeAddressCode& tac = intermediateCode[i];
            
            cout << "TAC: " << tac.result << " = ";
            if (tac.arg2.empty()) {
                cout << tac.op << " " << tac.arg1;
            } else {
                cout << tac.arg1 << " " << tac.op << " " << tac.arg2;
            }
            cout << endl;
            
            if (tac.op == "=") {
                // Simple assignment: result = arg1
                string src = getOperand(tac.arg1);
                string dest = allocateRegister(tac.result);
                
                if (src[0] == '#') {
                    // Load immediate
                    assemblyCode.push_back(AssemblyInstruction("MOV", dest, src));
                } else if (src[0] == 'R') {
                    // Register to register
                    assemblyCode.push_back(AssemblyInstruction("MOV", dest, src));
                } else {
                    // Load from memory
                    assemblyCode.push_back(AssemblyInstruction("LOAD", dest, src));
                }
                
                // If result is not a temporary, store to memory
                if (tac.result[0] != 't') {
                    assemblyCode.push_back(AssemblyInstruction("STORE", dest, tac.result));
                }
            }
            else if (tac.op == "+") {
                // Addition: result = arg1 + arg2
                string op1 = getOperand(tac.arg1);
                string op2 = getOperand(tac.arg2);
                string dest = allocateRegister(tac.result);
                
                // Load first operand
                if (op1[0] == '#') {
                    assemblyCode.push_back(AssemblyInstruction("MOV", dest, op1));
                } else if (op1[0] == 'R') {
                    if (op1 != dest) {
                        assemblyCode.push_back(AssemblyInstruction("MOV", dest, op1));
                    }
                } else {
                    assemblyCode.push_back(AssemblyInstruction("LOAD", dest, op1));
                }
                
                // Add second operand
                if (op2[0] == '#') {
                    assemblyCode.push_back(AssemblyInstruction("ADD", dest, op2));
                } else if (op2[0] == 'R') {
                    assemblyCode.push_back(AssemblyInstruction("ADD", dest, op2));
                } else {
                    string tempReg = allocateRegister("temp");
                    assemblyCode.push_back(AssemblyInstruction("LOAD", tempReg, op2));
                    assemblyCode.push_back(AssemblyInstruction("ADD", dest, tempReg));
                }
                
                // Store result if not temporary
                if (tac.result[0] != 't') {
                    assemblyCode.push_back(AssemblyInstruction("STORE", dest, tac.result));
                }
            }
            else if (tac.op == "*") {
                // Multiplication: result = arg1 * arg2
                string op1 = getOperand(tac.arg1);
                string op2 = getOperand(tac.arg2);
                string dest = allocateRegister(tac.result);
                
                // Load first operand
                if (op1[0] == '#') {
                    assemblyCode.push_back(AssemblyInstruction("MOV", dest, op1));
                } else if (op1[0] == 'R') {
                    if (op1 != dest) {
                        assemblyCode.push_back(AssemblyInstruction("MOV", dest, op1));
                    }
                } else {
                    assemblyCode.push_back(AssemblyInstruction("LOAD", dest, op1));
                }
                
                // Multiply by second operand
                if (op2[0] == '#') {
                    assemblyCode.push_back(AssemblyInstruction("MUL", dest, op2));
                } else if (op2[0] == 'R') {
                    assemblyCode.push_back(AssemblyInstruction("MUL", dest, op2));
                } else {
                    string tempReg = allocateRegister("temp");
                    assemblyCode.push_back(AssemblyInstruction("LOAD", tempReg, op2));
                    assemblyCode.push_back(AssemblyInstruction("MUL", dest, tempReg));
                }
                
                // Store result if not temporary
                if (tac.result[0] != 't') {
                    assemblyCode.push_back(AssemblyInstruction("STORE", dest, tac.result));
                }
            }
            else if (tac.op == "-") {
                // Subtraction: result = arg1 - arg2
                string op1 = getOperand(tac.arg1);
                string op2 = getOperand(tac.arg2);
                string dest = allocateRegister(tac.result);
                
                // Load first operand
                if (op1[0] == '#') {
                    assemblyCode.push_back(AssemblyInstruction("MOV", dest, op1));
                } else if (op1[0] == 'R') {
                    if (op1 != dest) {
                        assemblyCode.push_back(AssemblyInstruction("MOV", dest, op1));
                    }
                } else {
                    assemblyCode.push_back(AssemblyInstruction("LOAD", dest, op1));
                }
                
                // Subtract second operand
                if (op2[0] == '#') {
                    assemblyCode.push_back(AssemblyInstruction("SUB", dest, op2));
                } else if (op2[0] == 'R') {
                    assemblyCode.push_back(AssemblyInstruction("SUB", dest, op2));
                } else {
                    string tempReg = allocateRegister("temp");
                    assemblyCode.push_back(AssemblyInstruction("LOAD", tempReg, op2));
                    assemblyCode.push_back(AssemblyInstruction("SUB", dest, tempReg));
                }
                
                // Store result if not temporary
                if (tac.result[0] != 't') {
                    assemblyCode.push_back(AssemblyInstruction("STORE", dest, tac.result));
                }
            }
            
            cout << "Assembly:" << endl;
            // Print the assembly instructions generated for this TAC
            size_t start = assemblyCode.size();
            for (size_t j = max(0, (int)start - 4); j < assemblyCode.size(); j++) {
                cout << "  " << assemblyCode[j].toString() << endl;
            }
            cout << endl;
        }
    }
    
    void printIntermediateCode() {
        cout << "Three Address Code:" << endl;
        cout << string(30, '-') << endl;
        for (size_t i = 0; i < intermediateCode.size(); i++) {
            cout << i + 1 << ". " << intermediateCode[i].result << " = ";
            if (intermediateCode[i].arg2.empty()) {
                cout << intermediateCode[i].op << " " << intermediateCode[i].arg1;
            } else {
                cout << intermediateCode[i].arg1 << " " << intermediateCode[i].op 
                     << " " << intermediateCode[i].arg2;
            }
            cout << endl;
        }
        cout << endl;
    }
    
    void printAssemblyCode() {
        cout << "Final Assembly Code:" << endl;
        cout << string(30, '-') << endl;
        for (size_t i = 0; i < assemblyCode.size(); i++) {
            cout << i + 1 << ". " << assemblyCode[i].toString() << endl;
        }
        cout << endl;
    }
    
    void printRegisterAllocation() {
        cout << "Register Allocation:" << endl;
        cout << string(30, '-') << endl;
        for (const auto& pair : registerAllocation) {
            cout << pair.first << " -> " << pair.second << endl;
        }
        cout << endl;
    }
    
    void clear() {
        intermediateCode.clear();
        assemblyCode.clear();
        registerAllocation.clear();
        currentRegister = 0;
    }
};

int main() {
    cout << "Final Code Generator (Assembly Code Generation)" << endl;
    cout << "===============================================" << endl;
    
    FinalCodeGenerator generator;
    
    // Example 1: Simple arithmetic expression a = b + c * d
    cout << "Example 1: a = b + c * d" << endl;
    cout << "------------------------" << endl;
    
    generator.addIntermediateCode("*", "c", "d", "t1");
    generator.addIntermediateCode("+", "b", "t1", "t2");
    generator.addIntermediateCode("=", "t2", "", "a");
    
    generator.printIntermediateCode();
    generator.generateAssemblyCode();
    generator.printAssemblyCode();
    generator.printRegisterAllocation();
    
    generator.clear();
    cout << string(60, '=') << endl;
    
    // Example 2: More complex expression x = (a + b) * (c - d)
    cout << "Example 2: x = (a + b) * (c - d)" << endl;
    cout << "---------------------------------" << endl;
    
    generator.addIntermediateCode("+", "a", "b", "t1");
    generator.addIntermediateCode("-", "c", "d", "t2");
    generator.addIntermediateCode("*", "t1", "t2", "t3");
    generator.addIntermediateCode("=", "t3", "", "x");
    
    generator.printIntermediateCode();
    generator.generateAssemblyCode();
    generator.printAssemblyCode();
    generator.printRegisterAllocation();
    
    generator.clear();
    cout << string(60, '=') << endl;
    
    // Example 3: Assignment with constants
    cout << "Example 3: y = 5 * x + 10" << endl;
    cout << "-------------------------" << endl;
    
    generator.addIntermediateCode("*", "5", "x", "t1");
    generator.addIntermediateCode("+", "t1", "10", "t2");
    generator.addIntermediateCode("=", "t2", "", "y");
    
    generator.printIntermediateCode();
    generator.generateAssemblyCode();
    generator.printAssemblyCode();
    generator.printRegisterAllocation();
    
    return 0;
}
