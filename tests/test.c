#include <encryption.h>
#include <stdio.h>

int main(){
    struct encryptid_key* key = makeKey("whello");
    struct encryptid_session* session = makeSession(key);
    for (int x = 0; x < 200; x ++){
        printf("%d %d\n", encryptid_genOutKeybyte(session), encryptid_genInKeybyte(session));
    }
}