// Question 5: Write a program to calculate firstpos(), lastpos(), and followpos()

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
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

map<int, set<int>> followpos;
int pos_counter = 1;

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

void print_node_info(Node* node, const string& name) {
    if (!node) return;
    
    cout << name << ":" << endl;
    cout << "  Symbol: " << node->symbol;
    if (node->position != -1) cout << " (pos: " << node->position << ")";
    cout << endl;
    cout << "  Nullable: " << (node->nullable ? "true" : "false") << endl;
    cout << "  Firstpos: "; print_set(node->firstpos); cout << endl;
    cout << "  Lastpos: "; print_set(node->lastpos); cout << endl;
    cout << endl;
}

int main() {
    cout << "First, Last, and Follow Position Calculator" << endl;
    cout << "===========================================" << endl;
    
    // Example: (a|b)*abb
    // Parse tree construction for demonstration
    
    // Create leaf nodes
    Node* a1 = new Node('a', pos_counter++);  // position 1
    Node* b1 = new Node('b', pos_counter++);  // position 2
    Node* a2 = new Node('a', pos_counter++);  // position 3
    Node* b2 = new Node('b', pos_counter++);  // position 4
    Node* b3 = new Node('b', pos_counter++);  // position 5
    
    // Create internal nodes
    Node* or_node = new Node('|');
    or_node->left = a1;
    or_node->right = b1;
    
    Node* star_node = new Node('*');
    star_node->left = or_node;
    
    Node* concat1 = new Node('.');
    concat1->left = a2;
    concat1->right = b2;
    
    Node* concat2 = new Node('.');
    concat2->left = concat1;
    concat2->right = b3;
    
    Node* root = new Node('.');
    root->left = star_node;
    root->right = concat2;
    
    // Calculate nullable, firstpos, lastpos
    calculate_nullable(root);
    calculate_firstpos(root);
    calculate_lastpos(root);
    calculate_followpos(root);
    
    cout << "Regular Expression: (a|b)*abb" << endl;
    cout << "Positions: a(1), b(2), a(3), b(4), b(5)" << endl;
    cout << endl;
    
    // Print information for each node
    print_node_info(a1, "Node a(1)");
    print_node_info(b1, "Node b(2)");
    print_node_info(a2, "Node a(3)");
    print_node_info(b2, "Node b(4)");
    print_node_info(b3, "Node b(5)");
    print_node_info(or_node, "Node (a|b)");
    print_node_info(star_node, "Node (a|b)*");
    print_node_info(concat1, "Node ab");
    print_node_info(concat2, "Node abb");
    print_node_info(root, "Root (a|b)*abb");
    
    // Print followpos for each position
    cout << "FOLLOWPOS:" << endl;
    for (int i = 1; i < pos_counter; i++) {
        cout << "followpos(" << i << "): ";
        print_set(followpos[i]);
        cout << endl;
    }
    
    return 0;
}
