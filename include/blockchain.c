#include "../src/blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


// Create a new block
    Block* createBlock(int index, const char* data, const char* previousHash) {
    Block* newBlock = (Block*)malloc(sizeof(Block));
    // Set block properties
    newBlock->index = index;
    newBlock->timestamp = time(NULL);

    // Copy data safely
    strncpy(newBlock->data, data, 255);
    newBlock->data[255] = '\0';

    // Copy previous hash
    strncpy(newBlock->previousHash, previousHash, 64);
    newBlock->previousHash[64] = '\0';

    newBlock->nonce = 0;
    newBlock->next = NULL;

    // Calculate initial hash
    calculateHash(newBlock);

    return newBlock;
}

// Add block to end of chain
void insertBlock(Block** head, Block* newBlock) {
    if (!head || !newBlock) {
        printf("Error: Invalid parameters\n");
        return;
    }

    if (*head == NULL) {
        *head = newBlock;
    } else {
        Block* current = *head;
        // Find last block
        while (current->next != NULL) {
            current = current->next;
        }

        // Mine the block
        mineBlock(newBlock);

        // Link it to chain
        current->next = newBlock;
    }
}

// Simple hash
void calculateHash(Block* block) {
    char input[1024];

    // Combine all important data for hashing
    snprintf(input, sizeof(input), "%d%lld%s%s%d",
    block->index,
    (long long)block->timestamp,  // Add (long long) cast
    block->data,
    block->previousHash,
    block->nonce);

    // Simple hash algorithm
    unsigned long hash = 5381;
    for (int i = 0; input[i] != '\0'; i++) {
        hash = ((hash << 5) + hash) + input[i];
    }

    // Convert to hex string
    for (int i = 0; i < 16; i++) {
        sprintf(&block->hash[i * 4], "%04lx", hash);
        hash = hash * 1103515245 + 12345;  // Mix it up
    }
    block->hash[64] = '\0';
}

// Mine block with proof-of-work
void mineBlock(Block* block) {
    int difficulty = 2;  // Number of leading zeros required
    char target[5];

    // Create target string (e.g., "0000")
    for (int i = 0; i < difficulty; i++) {
        target[i] = '0';
    }
    target[difficulty] = '\0';

    printf("Mining block #%d... ", block->index);

    block->nonce = 0;

    // Proof of work loop
    do {
        block->nonce++;
        calculateHash(block);
    } while (strncmp(block->hash, target, difficulty) != 0);

    printf("Done! Nonce: %d\n", block->nonce);
}

// Validate entire blockchain
int validateChain(Block* head) {
    if (head == NULL) {
        printf("Blockchain is empty\n");
        return 0;
    }

    // Check genesis block
    // Should have index 0 and previous hash of all zeros
    if (head->index != 0) {
        printf("Error: First block should be genesis (index 0)\n");
        return 0;
    }

    // Check if genesis has all zeros as previous hash
    int is_all_zeros = 1;
    for (int i = 0; i < 64; i++) {
        if (head->previousHash[i] != '0') {
            is_all_zeros = 0;
            break;
        }
    }

    if (!is_all_zeros) {
        printf("Error: Genesis block should have all zeros as previous hash\n");
        return 0;
    }

    Block* current = head;
    int blocks_checked = 0;

    while (current != NULL) {
        blocks_checked++;

        // Save current values
        char original_hash[65];
        int original_nonce = current->nonce;
        strcpy(original_hash, current->hash);

        // Recalculate hash
        calculateHash(current);

        // Check if hash matches
        if (strcmp(current->hash, original_hash) != 0) {
            printf("Error: Block #%d has invalid hash\n", current->index);
            // Restore original values
            strcpy(current->hash, original_hash);
            current->nonce = original_nonce;
            return 0;
        }

        // Restore original values
        strcpy(current->hash, original_hash);
        current->nonce = original_nonce;

        // Check link to next block
        if (current->next != NULL) {
            if (strcmp(current->hash, current->next->previousHash) != 0) {
                printf("Error: Chain broken between block #%d and #%d\n",
                       current->index, current->next->index);
                return 0;
            }
        }

        current = current->next;
    }

    printf("Blockchain is valid! (%d blocks checked)\n", blocks_checked);
    return 1;
}

// Display blockchain
void displayBlockchain(Block* head) {
    if (head == NULL) {
        printf("Blockchain is empty\n");
        return;
    }

    printf("\n==================================================\n");
    printf("                   BLOCKCHAIN                     \n");
    printf("==================================================\n\n");

    Block* current = head;
    while (current != NULL) {
        printf("BLOCK #%d\n", current->index);
        printf("  Timestamp:  %lld\n", (long long)current->timestamp);
        printf("  Data:       %s\n", current->data);
        printf("  Nonce:      %d\n", current->nonce);
        printf("  Prev Hash:  %.12s...\n", current->previousHash);
        printf("  Hash:       %.12s...\n", current->hash);

        if (current->next != NULL) {
            printf("                     |\n");
            printf("                     v\n");
        }

        current = current->next;
    }
    printf("\n==================================================\n");
}

// Create genesis block
Block* makeGenesisBlock() {
    // Genesis block has all zeros as previous hash
    Block* genesis = createBlock(0, "Genesis Block",
        "0000000000000000000000000000000000000000000000000000000000000000");

    if (genesis) {
        printf("Genesis block created successfully\n");
    }

    return genesis;
}
// Initialize blockchain with genesis block
Block* initializeBlockchain() {
    printf("Initializing new blockchain...\n");
    return makeGenesisBlock();
}
// Free all memory - remove duplicate function
void freeBlockchain(Block* head) {
    Block* current = head;
    while (current != NULL) {
        Block* next = current->next;
        free(current);
        current = next;
    }

}
