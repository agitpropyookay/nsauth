# nsauth
cryptographic identity utility

## usage
try `$ nsauth --help` or `$ nsauth --usage` for more information.
- `$ nsauth --generate key.pem             # generate RSA pkey`
- `$ nsauth --challenge example key.pem    # sign a challenge`
- `$ nsauth -c example -g key.pem          # single invocation`

generated private keys are encrypted using the AES algorithm with a 256-bit key in CBC mode and stored in PEM format; you will be prompted for a password on encryption and decryption

## compilation
dependencies: `glibc`, `help2man`, `libssl-dev`, `make`  
compile with `$ make`  
your compiler must support C23
