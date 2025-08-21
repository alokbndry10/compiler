// Question 6: Write a program to calculate firstpos(), lastpos(), and followpos()

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
using namespace std;

// Node structure for parse tree
struct Node {
    char symbol;
    int position;
    bool nullable;
    set<int> firstpos;
    set<int> lastpos;
    Node* left;
    Node* right;
    
    Node(char c, int pos = -1) : symbol(c), position(pos), nullable(false), left(nullptr), right(nullptr) {}
};

class RegexAnalyzer {
private:
    map<int, set<int>> followpos;
    map<int, char> position_symbols;
    int pos_counter;
    
public:
    RegexAnalyzer() : pos_counter(1) {}
    
    void print_set(const set<int>& s) {
        cout << "{";
        bool first = true;
        for (int x : s) {
            if (!first) cout << ", ";
            cout << x;
            first = false;
        }
        cout << "}";
    }
    
    void calculate_nullable(Node* node) {
        if (!node) return;
        
        calculate_nullable(node->left);
        calculate_nullable(node->right);
        
        switch (node->symbol) {
            case '*':
                node->nullable = true;
                break;
            case '|':
                node->nullable = node->left->nullable || node->right->nullable;
                break;
            case '.':
                node->nullable = node->left->nullable && node->right->nullable;
                break;
            default:
                node->nullable = false;
        }
    }
    
    void calculate_firstpos(Node* node) {
        if (!node) return;
        
        calculate_firstpos(node->left);
        calculate_firstpos(node->right);
        
        switch (node->symbol) {
            case '*':
                node->firstpos = node->left->firstpos;
                break;
            case '|':
                node->firstpos = node->left->firstpos;
                node->firstpos.insert(node->right->firstpos.begin(), node->right->firstpos.end());
                break;
            case '.':
                node->firstpos = node->left->firstpos;
                if (node->left->nullable) {
                    node->firstpos.insert(node->right->firstpos.begin(), node->right->firstpos.end());
                }
                break;
            default:
                if (node->position != -1) {
                    node->firstpos.insert(node->position);
                }
        }
    }
    
    void calculate_lastpos(Node* node) {
        if (!node) return;
        
        calculate_lastpos(node->left);
        calculate_lastpos(node->right);
        
        switch (node->symbol) {
            case '*':
                node->lastpos = node->left->lastpos;
                break;
            case '|':
                node->lastpos = node->left->lastpos;
                node->lastpos.insert(node->right->lastpos.begin(), node->right->lastpos.end());
                break;
            case '.':
                node->lastpos = node->right->lastpos;
                if (node->right->nullable) {
                    node->lastpos.insert(node->left->lastpos.begin(), node->left->lastpos.end());
                }
                break;
            default:
                if (node->position != -1) {
                    node->lastpos.insert(node->position);
                }
        }
    }
    
    void calculate_followpos(Node* node) {
        if (!node) return;
        
        if (node->symbol == '.') {
            // For concatenation, followpos of lastpos(left) includes firstpos(right)
            for (int pos : node->left->lastpos) {
                followpos[pos].insert(node->right->firstpos.begin(), node->right->firstpos.end());
            }
        } else if (node->symbol == '*') {
            // For Kleene star, followpos of lastpos includes firstpos
            for (int pos : node->lastpos) {
                followpos[pos].insert(node->firstpos.begin(), node->firstpos.end());
            }
        }
        
        calculate_followpos(node->left);
        calculate_followpos(node->right);
    }
    
    Node* parseRegex(const string& regex) {
        // Simple parser for basic regex patterns
        // This is a simplified version - handles basic concatenation, alternation, and Kleene star
        
        stack<Node*> nodes;
        stack<char> operators;
        
        for (size_t i = 0; i < regex.length(); i++) {
            char c = regex[i];
            
            if (isalpha(c)) {
                // Create leaf node for alphabet symbol
                Node* leaf = new Node(c, pos_counter);
                position_symbols[pos_counter] = c;
                pos_counter++;
                nodes.push(leaf);
            }
            else if (c == '|') {
                // Alternation operator
                if (nodes.size() >= 2) {
                    Node* right = nodes.top(); nodes.pop();
                    Node* left = nodes.top(); nodes.pop();
                    Node* or_node = new Node('|');
                    or_node->left = left;
                    or_node->right = right;
                    nodes.push(or_node);
                }
            }
            else if (c == '*') {
                // Kleene star operator
                if (!nodes.empty()) {
                    Node* operand = nodes.top(); nodes.pop();
                    Node* star_node = new Node('*');
                    star_node->left = operand;
                    nodes.push(star_node);
                }
            }
            else if (c == '.') {
                // Concatenation operator
                if (nodes.size() >= 2) {
                    Node* right = nodes.top(); nodes.pop();
                    Node* left = nodes.top(); nodes.pop();
                    Node* concat_node = new Node('.');
                    concat_node->left = left;
                    concat_node->right = right;
                    nodes.push(concat_node);
                }
            }
        }
        
        return nodes.empty() ? nullptr : nodes.top();
    }
    
    void print_node_info(Node* node, const string& name) {
        if (!node) return;
        
        cout << name << ":" << endl;
        cout << "  Symbol: " << node->symbol;
        if (node->position != -1) {
            cout << " (pos: " << node->position << " = '" << position_symbols[node->position] << "')";
        }
        cout << endl;
        cout << "  Nullable: " << (node->nullable ? "true" : "false") << endl;
        cout << "  Firstpos: "; print_set(node->firstpos); cout << endl;
        cout << "  Lastpos: "; print_set(node->lastpos); cout << endl;
        cout << endl;
    }
    
    void analyzeRegex(const string& regex) {
        cout << "Analyzing Regular Expression: " << regex << endl;
        cout << string(50, '-') << endl;
        
        // Reset counters
        pos_counter = 1;
        followpos.clear();
        position_symbols.clear();
        
        Node* root = parseRegex(regex);
        
        if (!root) {
            cout << "Error: Invalid regular expression!" << endl;
            return;
        }
        
        // Calculate nullable, firstpos, lastpos
        calculate_nullable(root);
        calculate_firstpos(root);
        calculate_lastpos(root);
        calculate_followpos(root);
        
        // Print position mapping
        cout << "Position Mapping:" << endl;
        for (const auto& pair : position_symbols) {
            cout << "Position " << pair.first << ": '" << pair.second << "'" << endl;
        }
        cout << endl;
        
        // Print root information
        print_node_info(root, "Root Node");
        
        // Print followpos for each position
        cout << "FOLLOWPOS:" << endl;
        for (int i = 1; i < pos_counter; i++) {
            cout << "followpos(" << i << "): ";
            print_set(followpos[i]);
            cout << " [symbol: '" << position_symbols[i] << "']" << endl;
        }
        cout << endl;
    }
};

int main() {
    cout << "First, Last, and Follow Position Calculator" << endl;
    cout << "===========================================" << endl;
    cout << "Enter regular expressions to analyze" << endl;
    cout << "Supported operators: . (concat), | (alternation), * (Kleene star)" << endl;
    cout << "Example formats: a.b, a|b, a*, a.b*, (a|b).a.b.b" << endl;
    cout << "Note: Use explicit . for concatenation" << endl << endl;
    
    RegexAnalyzer analyzer;
    string regex;
    char choice;
    
    do {
        cout << "Enter regular expression: ";
        cin >> regex;
        
        cout << "\n";
        analyzer.analyzeRegex(regex);
        
        cout << "Do you want to analyze another regex? (y/n): ";
        cin >> choice;
        cout << endl;
        
    } while (choice == 'y' || choice == 'Y');
    
    return 0;
}
