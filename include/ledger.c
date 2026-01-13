#include "../src/ledger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for file storage (without pointer)
typedef struct {
    int index;
    time_t timestamp;
    char data[256];
    char hash[65];
    char previousHash[65];
    int nonce;
} BlockData;

// Save entire blockchain to binary file
int saveBlockchainToFile(Block* head, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error: Cannot open %s for writing\n", filename);
        return 0;
    }
    
    Block* current = head;
    BlockData blockData;
    int blockCount = 0;
    
    while (current != NULL) {
        // Copy block data (excluding pointer)
        blockData.index = current->index;
        blockData.timestamp = current->timestamp;
        strncpy(blockData.data, current->data, 255);
        blockData.data[255] = '\0';
        strncpy(blockData.hash, current->hash, 64);
        blockData.hash[64] = '\0';
        strncpy(blockData.previousHash, current->previousHash, 64);
        blockData.previousHash[64] = '\0';
        blockData.nonce = current->nonce;
        
        // Write to file
        if (fwrite(&blockData, sizeof(BlockData), 1, file) != 1) {
            printf("Error writing block %d to file\n", current->index);
            fclose(file);
            return 0;
        }
        
        current = current->next;
        blockCount++;
    }
    
    fclose(file);
    printf("Saved %d blocks to %s\n", blockCount, filename);
    return 1;
}

// Load blockchain from binary file
Block* loadBlockchainFromFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: Cannot open %s for reading\n", filename);
        return NULL;
    }
    
    Block* head = NULL;
    Block* tail = NULL;
    BlockData blockData;
    int blockCount = 0;
    
    while (fread(&blockData, sizeof(BlockData), 1, file) == 1) {
        // Create new block
        Block* newBlock = (Block*)malloc(sizeof(Block));
        if (!newBlock) {
            printf("Error: Memory allocation failed for block %d\n", blockCount);
            // Free what we've already allocated
            Block* current = head;
            while (current) {
                Block* next = current->next;
                free(current);
                current = next;
            }
            fclose(file);
            return NULL;
        }
        
        // Copy data from file structure
        newBlock->index = blockData.index;
        newBlock->timestamp = blockData.timestamp;
        strncpy(newBlock->data, blockData.data, 255);
        newBlock->data[255] = '\0';
        strncpy(newBlock->hash, blockData.hash, 64);
        newBlock->hash[64] = '\0';
        strncpy(newBlock->previousHash, blockData.previousHash, 64);
        newBlock->previousHash[64] = '\0';
        newBlock->nonce = blockData.nonce;
        newBlock->next = NULL;
        
        // Add to linked list
        if (head == NULL) {
            head = newBlock;
            tail = newBlock;
        } else {
            tail->next = newBlock;
            tail = newBlock;
        }
        
        blockCount++;
    }
    
    fclose(file);
    
    if (blockCount == 0) {
        printf("No blocks found in %s\n", filename);
        return NULL;
    }
    
    printf("Loaded %d blocks from %s\n", blockCount, filename);
    return head;
}

// Append a single block to ledger file
int appendBlockToLedger(Block* block, const char* filename) {
    FILE* file = fopen(filename, "ab");
    if (file == NULL) {
        printf("Error: Cannot open %s for appending\n", filename);
        return 0;
    }
    
    BlockData blockData;
    
    // Prepare data (without pointer)
    blockData.index = block->index;
    blockData.timestamp = block->timestamp;
    strncpy(blockData.data, block->data, 255);
    blockData.data[255] = '\0';
    strncpy(blockData.hash, block->hash, 64);
    blockData.hash[64] = '\0';
    strncpy(blockData.previousHash, block->previousHash, 64);
    blockData.previousHash[64] = '\0';
    blockData.nonce = block->nonce;
    
    // Append to file
    if (fwrite(&blockData, sizeof(BlockData), 1, file) != 1) {
        printf("Error appending block %d to file\n", block->index);
        fclose(file);
        return 0;
    }
    
    fclose(file);
    printf("Appended block %d to %s\n", block->index, filename);
    return 1;
}