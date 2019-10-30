# XOR cipher

XOR is exclusive-or operation with bits:
```math
0 • 0 = 0
0 • 1 = 1
1 • 0 = 0
1 • 1 = 0 
```
It's nothing more than [Vigenere](https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher) polyalphabetic cipher.
This cipher can encrypt a plain text with keyword (it means password). Encrypt and decrypt are made by [tabula recta](https://en.wikipedia.org/wiki/Tabula_recta). This table contains the English alphabet shifted by some value. We can match plain text symbol with keyword and to get cipher text.

In XOR cipher we have keyword too. And xor-algorithm is symmetric algorithm too. There are some common formula symmetric ciphers:
```math
P • K = C
C • K = P

P - plain text
C - cipher text
K - key
```
It's sufficient theoretical foundations to code your own first xor-cipher.

### How to build
It's very easy:
```sh
git clone git@github.com:anisyanka/RnD-new-entity.git
cd simple-xor
make
```
In the directiry you can see the executable file `xor_enc.a`.

### How to use
```
Usage: ./xor_enc.a <flags> -i [FILE] -k [KEYWORD]

flags:
-i --> input file pointer
-k --> keyword pointer
-t --> test mode (if it is on, other options are ignored)
-e --> encrypt [FILE] with [KEYWORD]
-d --> decrypt [FILE] with [KEYWORD]
-b --> break xored [FILE] ([KEYWORD] isn't used in this case)
```

### How to work break algorithm
In modern computer world anyone can write the break algorithm for XOR.
By itself, using a constant repeating key, a simple XOR cipher can trivially be broken using [frequency analysis](https://en.wikipedia.org/wiki/Frequency_analysis).
XOR cipher has key length less than plait text size. We can use the fact to break XOR.
