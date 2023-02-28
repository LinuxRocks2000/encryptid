#include <encryption.h>
#include <stdlib.h>
#include <string.h>


// Internal function that uses a key and counter and generates a kb
uint8_t encryptid_genKeybyte(struct encryptid_key* key, unsigned long* byteCounter, uint8_t* last){
    uint8_t rawKB = key -> key[*byteCounter % sizeof(key -> key)];
    //rawKB |= *byteCounter;
    //rawKB &= ~((*last * *last) >> 1);
    *last = rawKB;
    byteCounter ++; // Increment the byte count
    return rawKB;
}

uint8_t encryptid_genOutKeybyte(struct encryptid_session* session){
    return encryptid_genKeybyte(session -> key, &session -> outCount, &session -> key -> lastOut);
}

uint8_t encryptid_genInKeybyte(struct encryptid_session* session){
    return encryptid_genKeybyte(session -> key, &session -> inCount, &session -> key -> lastIn);
}

struct encryptid_session* makeSession(struct encryptid_key* key){
    struct encryptid_session* e = (struct encryptid_session*)malloc(sizeof(struct encryptid_session));
    e -> key = key;
    e -> inCount = 0;
    e -> outCount = 0;
    return e;
}

struct encryptid_key* makeKey(char in[KEYLENGTH + 1]){
    struct encryptid_key* key = (struct encryptid_key*)malloc(sizeof(struct encryptid_key));
    key -> lastOut = in[0];
    key -> lastIn = in[0];
    memcpy(key -> key, in + 1, KEYLENGTH);
    return key;
}