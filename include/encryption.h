#pragma once
/*
 * Full implementation of the encryptid protocol
 * Encryption functions and types
 */

#include <stdint.h>
#define KEYLENGTH 256


struct encryptid_key {
    uint8_t last;
    char key[KEYLENGTH];
};

struct encryptid_session {
    struct encryptid_key* key;
    unsigned long inCount;
    unsigned long outCount;
};

struct encryptid_session* makeSession(struct encryptid_key* key);

uint8_t encryptid_genKeybyte(struct encryptid_session* key);
