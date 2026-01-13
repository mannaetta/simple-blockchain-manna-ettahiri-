#ifndef LEDGER_H
#define LEDGER_H

#include "blockchain.h"

#define LEDGER_FILE "blockchain_ledger.dat"

// Function prototypes for ledger operations
int saveBlockchainToFile(Block* head, const char* filename);
Block* loadBlockchainFromFile(const char* filename);
int appendBlockToLedger(Block* block, const char* filename);

#endif