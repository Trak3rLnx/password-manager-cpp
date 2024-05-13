## CLI Password Manager write in C++

### Usage
! For each action, PassMan will asks you your passphrase ! 

To search a password
```bash
./passman <website you looking for> <id to copy>
```

To create your password file encrypted with your passphrase
```bash
./passman init
```

To add a new entry in your password file
```bash
./passman add
```
It's going to ask you :
- Website
- Username / Email
- Password

### Features
Instant copy in your clipboard -> the password is never shown

### Depedencies
OpenSSL
Crypto lib C++
XClip

### Installation

```bash
g++ main.cpp -o passman -lssl -lcrypto
```
