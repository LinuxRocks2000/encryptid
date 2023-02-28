#include <encryption.h>
#include <stdlib.h>


uint8_t encryptid_genKeybyte(struct encryptid_session* session){
    return session -> key -> key[session -> outCount % sizeof(session -> key -> key)];
}

struct encryptid_session* makeSession(struct encryptid_key* key){
    struct encryptid_session* e = (struct encryptid_session*)malloc(sizeof(struct encryptid_session));
    e -> key = key;
    e -> inCount = 0;
    e -> outCount = 0;
    return e;
}