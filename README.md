> [!NOTE]
> The corresponding C project is not interoperable with nsauth web.  This is due to change in the direction of the project.  This may be solved in future.

# nsauth
cryptographic identity utility

## usage
Download `nsauth.html` and open in a modern browser,[^1] or visit a mirror.[^2]

Create a signature:
1. make a new key pair or paste an existing one in the `Key Pair' textarea
2. enter your challenge in the `Challenge' text input
3. sign
4. send your public key and signature to your service operator

Verify a signature:
1. enter the signature, corresponding public key, and challenge
2. check the verification icon
   - if Google Fonts is unavailable, then you may see \`VERIFIED' (valid) or \`VERIFIED_OFF' (invalid)
[^1]: effort will be made to support Ladybird after its Alpha release in 2026
[^2]: mirrors are convenient but may serve a malicious page

## opsec considerations
Effort has been made to make usage secure, e.g. by disabling spellcheck, but this is not foolproof.  Users must ensure no untrusted extensions have access to the page, no malware is installed on their device, and any key pairs are stored securely e.g. in a password database or encrypted volume.  If you use a mirror, then check both the connection is secure and you trust the provider.[^2]

## mirrors
- soon&trade;
