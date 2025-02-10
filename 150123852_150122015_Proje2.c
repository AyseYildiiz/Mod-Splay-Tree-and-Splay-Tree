/* ÝLAYDA ÝLHAN 150123852
   AYÞE YILDIZ 150122015
   In this code, costs are calculated in the splay tree and its modified version, mod-splay tree. */

/* Standart C libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A struct is created for the data structure and definitions.*/
typedef struct node {
    char data;
    int occurence, rot_cost, comp_cost;
    struct node *right, *left, *parent;
} Node;

/* A new node is created with the createNode function.*/
Node* createNode(char key, Node* parent) {
    Node* newnode = (Node*)malloc(sizeof(Node));
    newnode->data = key;
    newnode->occurence = 0;
    newnode->rot_cost = 0;
    newnode->comp_cost = 0;
    newnode->parent = parent;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

/* The find function searches for a node with a specific key value in the tree.*/
Node* find(Node* t, char key) {
    if (t != NULL) {
        if (key < t->data) {
            t->comp_cost++;
            return find(t->left, key);
        } else if (key > t->data) {
            t->comp_cost++;
            return find(t->right, key);
        } else {
            t->occurence++;
            t->comp_cost++;
            return t;
        }
    } else {
        return NULL;
    }
}

/* This function performs left rotation.*/
void leftRotation(Node **root, Node *x) {
    Node *y = x->right;
    if (y) {
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
    }

    if (!x->parent)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    if (y)
        y->left = x;
    x->parent = y;
    x->rot_cost++;
}

/* This function performs right rotation.*/
void rightRotation(Node **root, Node *x) {
    Node *y = x->left;
    if (y) {
        x->left = y->right;
        if (y->right) y->right->parent = x;
        y->parent = x->parent;
    }

    if (!x->parent)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    if (y)
        y->right = x;
    x->parent = y;
    x->rot_cost++;
}

/* In this function, a splay tree is obtained by performing splay tree rotations.*/
void splay(Node **root, Node *x) {
    while (x->parent) {
        if (!x->parent->parent) {
            if (x->parent->left == x)
                rightRotation(root, x->parent);
            else
                leftRotation(root, x->parent);
        } else if (x->parent->left == x && x->parent->parent->left == x->parent) {
            rightRotation(root, x->parent->parent);
            rightRotation(root, x->parent);
        } else if (x->parent->right == x && x->parent->parent->right == x->parent) {
            leftRotation(root, x->parent->parent);
            leftRotation(root, x->parent);
        } else if (x->parent->left == x && x->parent->parent->right == x->parent) {
            rightRotation(root, x->parent);
            leftRotation(root, x->parent);
        } else {
            leftRotation(root, x->parent);
            rightRotation(root, x->parent);
        }
    }
}

/* In this function, a modified version of the splay tree, the modsplay tree, is created.*/
Node* modSplay(Node* key_tree, Node* tree) {
    if (key_tree != NULL && key_tree->occurence > tree->occurence) {
        splay(&tree, key_tree);
       
    }
    return tree;
}

/* This function adds a node to the splay tree.*/
Node* insert(Node** root, char key) {
    Node *t = *root;
    Node *p = NULL;

    while (t) {
        p = t;
        t = (t->data < key) ? t->right : t->left;
    }

    t = createNode(key, p);
    if (!p)
        *root = t;
    else if (p->data < key)
        p->right = t;
    else
        p->left = t;

    splay(root, t);
    return t;
}

/* With this function, a new node is created and the created node is added to the tree.*/
Node* insertMod(Node** root, char key) {
    Node *t = *root;
    Node *p = NULL;

    while (t) {
        p = t;
        t = (t->data < key) ? t->right : t->left;
    }

    t = createNode(key, p);
    if (!p)
        *root = t;
    else if (p->data < key)
        p->right = t;
    else
        p->left = t;

    return t;
}

/* With this function, the character information of the node is written as preorder.*/
void printTree(Node* t, FILE* output) {
    if (t != NULL) {
        fprintf(output, "%c ", t->data);
        printTree(t->left, output);
        printTree(t->right, output);
    }
}

/* With this function, the character information and occurrence of the node are written as preorder.*/
void printModTree(Node* t, FILE* output) {
    if (t != NULL) {
        fprintf(output, "%c(%d) ", t->data, t->occurence);
        printModTree(t->left, output);
        printModTree(t->right, output);
    }
}

/* This function collects the comparison cost of each node.*/
int comparisonCost(Node* tree) {
    if (tree == NULL) return 0;
    return tree->comp_cost + comparisonCost(tree->left) + comparisonCost(tree->right);
}
/* This function collects the rotation cost of each node.*/
int rotationCost(Node* tree) {
    if (tree == NULL) return 0;
    return tree->rot_cost + rotationCost(tree->left) + rotationCost(tree->right);
}

/* In the main function, two different trees are created and 
their costs are calculated and printed to an output file.*/
int main() {
    FILE *input_file = fopen("input.txt", "r");  
    FILE *output_file = fopen("output.txt", "w");  
    char line[1000];
    Node* t = NULL;
    int total_cost_splay = 0;
    int total_cost_modSplay = 0;

    while (fgets(line, sizeof(line), input_file)) {
        char* token = strtok(line, ",");  
        while (token != NULL) {
            char ch = token[0];
            if (t == NULL) {
                insert(&t, ch);
            } else {
                Node* m = find(t, ch);
                if (m == NULL) {
                    insert(&t, ch);
                } else {
                    splay(&t, m);
                }
            }
            token = strtok(NULL, ",");  
        }
    }
    total_cost_splay += comparisonCost(t) + rotationCost(t);
    fprintf(output_file, "Splay Tree: ");
    printTree(t, output_file);
    fprintf(output_file, "\nTotal Cost for Splay Tree: %d\n", total_cost_splay);
    t = NULL;
    rewind(input_file);  
    while (fgets(line, sizeof(line), input_file)) {
        char* token = strtok(line, ",");  
        while (token != NULL) {
            char ch = token[0];  

            if (t == NULL) {
                insertMod(&t, ch);
            } else {
                Node* m = find(t, ch);
                if (m == NULL) {
                    insertMod(&t, ch);
                } else {
                    t = modSplay(m, t);
                }
            }

            token = strtok(NULL, ",");
        }
    }
    total_cost_modSplay += comparisonCost(t) + rotationCost(t);
    fprintf(output_file, "Mod Splay Tree: ");
    printModTree(t, output_file);
    fprintf(output_file, "\nTotal Cost for Mod Splay Tree: %d\n", total_cost_modSplay);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
