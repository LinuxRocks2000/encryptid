#pragma once
/*
 * Full implementation of the encryptid protocol
 * Encryption functions and types
 */

#include <stdint.h>
#define KEYLENGTH 6


struct encryptid_key {
    uint8_t lastIn; // Last decrypted input byte
    uint8_t lastOut; // Last encrypted output byte - starts as the first byte of the key
    char key[KEYLENGTH];
};

struct encryptid_session {
    struct encryptid_key* key;
    unsigned long inCount;
    unsigned long outCount;
};

struct encryptid_session* makeSession(struct encryptid_key* key);

struct encryptid_key* makeKey(char in[KEYLENGTH + 1]);

uint8_t encryptid_genOutKeybyte(struct encryptid_session* session);

uint8_t encryptid_genInKeybyte(struct encryptid_session* session);
