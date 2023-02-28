# Encryptid

A protocol (and implementation, and library) for E2E encryption and SSL-like authentication. Works on port 9764 by default. It's a forwarding protocol - outside network traffic that wishes to make an encrypted and authenticated connection makes a request to the forwarding server, which decrypts the encrypted communications and forwards them raw to the target server on the host.

## Specification

A client that wishes to connect to an encryptid network should connect to the encryptid port and send the "entry header". This is the word "ECYTD" (every other letter of ENCRYPTID) and a single byte containing the following bit flags:

0. WILL_ENCRYPT: If this is set, the client will encrypt communications. If unset, the client will be treated as a guest user, to prevent sensitive data being spread over unencrypted communications (Please do not use this).
1. All further bits including this one are reserved for future use.

The server should respond with an error byte (see below); if the error byte is OK, the server and client begin the Diffie-Hellman like process:

* Both parties generate a random 2056-bit (257 byte - we reserve the first byte) private key.
* Server generates a random public key and sends it to the client.
* Server uses bitwise OR to sign its private key by public, and sends that to the client.
* Client uses bitwise OR to sign its private key by public and sends that to the server.
* Server signs what the client sent it by its private key with bitwise OR.
* Client signs what the server sent it by its private key with bitwise OR.

Both parties should now have the same 2056-bit private key (note: this is a very new project, and I'm still playing around with the encryption process, so this is subject to change and hopefully improvement).

### Encryption

If encryption is desired,

Each side should remove the first byte of the key and put it in a one-byte buffer, and also create an incoming buffer. Each side should also start two byte counters - one for incoming, one for outgoing. Every time a byte is sent, increment the outgoing counter, and every time a byte is received, increment the incoming counter.

To get the next key byte, take the binary OR of the byte buffer and the byte counter, and then binary AND that result against the byte in the key given by [byte counter - incoming or outgoing, depending on context] mod [length]. Set the byte buffer to that value XOR the data byte, and broadcast at will. To decrypt, perform exactly the same process, but on an encrypted byte instead of the data byte.

### Packets

Packets start with a byte - if the first bit is set, then it is an informative packet (such as for authentication), otherwise it is the length of the packet. In the second case, everything following is treated as data (Note: everything, including the packet header byte, is encrypted, assuming encryption is on).

If it is an informative byte, the next 7 bits are an operation code. Valid codes:

0. Status OK: Everything's fine, thanks
1. Status REJECTED: A request was rejected.
2. Reserved for future use
3. Reserved for future use
4. Reserved for future use
5. Reserved for future use
6. Reserved for future use
7. Reserved for future use
8. OFFER AUTHENTICATION: Next byte contains the client's authentication parameters. First bit of auth parameters is whether or not it provides a username - the client is willing to provide a username (this should not be sufficient under most circumstances). Second bit is whether or not it provides a password (this does not necessarily require a username!), the third bit is whether or not it can provide a universal authentication token (very strict servers will automatically reject clients that do not provide a valid token, whether or not they provide a username and password), and the rest are reserved. Once authentication is offered, the client should wait for the server to send either OK or REJECTED: if it sends OK, the client should broadcast the credentials it said it can offer, then send AUTHENTICATION COMPLETE. Servers should only REJECT client offers if they don't provide sufficient credentials; extra authentication credentials should just be ignored. There is a possible Man-In-The-Middle attack here; a revised edition with SSH-like MITM protection will be released later.
9. OFFER USERNAME: The next data packet contains a username.
10. OFFER PASSWORD: The next data packet contains a password.
11. OFFER TOKEN: The next data packet contains a token; this token is implementation defined and can be anything from a randomly generated string shared to all clients to a proper SSH key.
12. AUTHENTICATION COMPLETE: The client is done broadcasting credentials and will now await the server's approval or disapproval. If the server returns REJECTED, both sides should immediately disconnect. If it returns OK, the session is now considered authenticated.
13. Reserved for future use
14. Reserved for future use
15. Reserved for future use
16. EXTENDED FRAME 16 BIT: The next two bytes are the packet length. Allows for up to 64 kilobytes in a packet. May be REJECTED if the server does not wish to buffer the requested size, but this is implementation defined (if REJECTED, the client should re-broadcast in normal packet format).
17. EXTENDED FRAME 32 BIT: The next four bytes are the packet length. Up to 4 gigabytes. See above.
18. EXTENDED FRAME 64 BIT: The next 8 bytes are the packet length. Up to 16 exabytes. In the distant future I hope somebody will have a good reason to use this.
19. CONNECT TO PORT: Authentication is complete and the client would like to connect to a port. If authentication has not been performed before this is sent, authentication is considered complete as a guest. The server should consult authentication rules - if the authenticated user does not have permission to access that port, it should REJECT and close. If the authenticated user *does* have permission to access that port, the server should tunnel unencrypted data to that port (unencrypted is a matter of perspective - the client and endpoint server may do their own encryption, if they wish to).
20. DISCONNECT: This is implied by tcp disconnection. Close the session with the endpoint server and disconnect from encryptid.
21. Everything past this point is reserved for future use.