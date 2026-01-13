#include <stdio.h>
#include <stdlib.h>
#include "../src/linkedlist.h"
#include "../src/blockchain.h"
#include "../src/ledger.h"

Block* initializeBlockchain();  // Tell main.c this function exists
void testLinkedList() {
    printf("\n=== Testing Linked List ===\n");

    Node* list = NULL;

    // Create and insert nodes
    list = insertNode(list, 10);
    list = insertNode(list, 20);
    list = insertNode(list, 30);
    list = insertNode(list, 40);


    // Display list
    displayList(list);

    // Delete a node
    list = deleteNode(list, 20);
    printf("After deleting 20:\n");
    displayList(list);

    // Free list
    freeList(list);
}

void testBlockchain() {
    printf("\n=== Testing Blockchain ===\n");

    Block* blockchain = NULL;

    //Initialize blockchain
    blockchain = initializeBlockchain();

    //Create and mine new blocks
    Block* block1 = createBlock(1, "First transaction: Alice to Bob 10 BTC", blockchain->hash);
    mineBlock(block1);
    insertBlock(&blockchain, block1);

    Block* block2 = createBlock(2, "Second transaction: Bob to Charlie 5 BTC", block1->hash);
    mineBlock(block2);
    insertBlock(&blockchain, block2);

    Block* block3 = createBlock(3, "Third transaction: Charlie to David 3 BTC", block2->hash);
    mineBlock(block3);
    insertBlock(&blockchain, block3);

    //Display blockchain
    displayBlockchain(blockchain);

    //Validate blockchain
    printf("\n=== Validating Blockchain ===\n");
    int isValid = validateChain(blockchain);
    printf("Blockchain is %s\n", isValid ? "VALID" : "INVALID");

    //Save to ledger
    printf("\n=== Testing Ledger Functions ===\n");
    saveBlockchainToFile(blockchain, LEDGER_FILE);

    // Test loading from ledger
    printf("\n=== Loading from Ledger ===\n");
    Block* loadedBlockchain = loadBlockchainFromFile(LEDGER_FILE);
    if (loadedBlockchain) {
        displayBlockchain(loadedBlockchain);
        freeBlockchain(loadedBlockchain);
    }

    //Clean up
    freeBlockchain(blockchain);
}

int main() {
    printf("C Blockchain Implementation Project\n");
    printf("===================================\n");

    //Test linked list
    testLinkedList();

    //Test blockchain
    testBlockchain();

    printf("\n=== All tests completed ===\n");
    return 0;
}
