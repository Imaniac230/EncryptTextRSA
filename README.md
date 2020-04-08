# EncryptTextRSA

Small programs for encrypting ASCII text file messages with RSA keys.

## Description

* A simple attempt at creating programs for encrypting and decrypting text file messages with RSA.
* Definitely **NOT** a secure implementation!
* **P** and **Q** primes are basically limited to **226 < P * Q < 255** in order to work with full ASCII range.
* Decryption works by factoring **P** and **Q**.
* Created only for educational purposes.

## Platforms
* C++ version tested in Windows 10 with Visual Studio 2017 (v141-default ~~and v141-clang-c2~~ compiler~~s~~)
* GO version tested in Windows 10 with Visual Studio Code (go default compiler)
