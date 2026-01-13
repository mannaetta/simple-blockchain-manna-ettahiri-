#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <time.h>

typedef struct Block {
    int index;
    time_t timestamp;
    char data[256];
    char hash[65];
    char previousHash[65];  // Fixed: capital H
    int nonce;
    struct Block* next;
} Block;

// Add ALL function declarations
Block* createBlock(int index, const char* data, const char* previousHash);
void insertBlock(Block** head, Block* newBlock);
void calculateHash(Block* block);
void mineBlock(Block* block);  // Remove difficulty parameter
int validateChain(Block* head);
void displayBlockchain(Block* head);
Block* initializeBlockchain();  // Returns Block* not void
void freeBlockchain(Block* head);

#endif