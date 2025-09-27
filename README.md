# nsauth
cryptographic identity utility

## usage
try `$ nsauth --help` or `$ nsauth --usage` for more information.
- `$ nsauth --generate key.pem             # generate RSA pkey`
- `$ nsauth --challenge example key.pem    # sign challenge`
- `$ nsauth -c example -g key.pem          # single invocation`

generated private keys are encrypted using the AES algorithm with a 256-bit key in CBC mode and stored in PEM format; you will be prompted for a password on encryption and decryption

## configuration
RSA modulus size BITS is hard coded; you may change it if you know what you are doing
|         bits | time[^1]             | description                                |
|--------------|----------------------|--------------------------------------------|
|          829 | 0m0.008s             | RSA250 factored Feb 28 2020                |
|         1024 | 0m0.012s             | insecure for signing as per NIST SP 800-57 |
| [2048, 8192] | [0m0.040s, 0m4.126s] | recommended                                |
|        16384 | 1m4.129s             | max recommended by OpenSSL                 |
[^1]: key generation user time on AMD Ryzen™ 7 9800X3D × 16

## compilation
dependencies: `glibc`, `help2man`, `libssl-dev`, `make`  
compile with `$ make`  
your compiler must support C23
