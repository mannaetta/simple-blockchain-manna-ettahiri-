//Define a struct for a linked list node.
typedef struct Node {
    int data;               // Data stored in the node
    struct Node* next;      // Pointer to the next node
} Node;

//Declare basic linked list operations.
Node* createNode(int data);                // Create a new node
Node* insertNode(Node* head, int data);    // Insert node at end
Node* deleteNode(Node* head, int data);    // Delete node with given data
void displayList(Node* head);              // Display the list
void freeList(Node* head);

