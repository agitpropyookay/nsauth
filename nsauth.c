/* See LICENSE file for license details.
 *
 * Please use a code formatter set to the GNU style when contributing; GNU
 * style is widely preset.  RSA modulus size BITS is hard coded; you may change
 * it if you know what you are doing.
 *
 * If you are a user, then please backup your keys!
 */

#define _GNU_SOURCE

#include <argp.h>
#include <fcntl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* types */
struct arguments
{
  char *arg, *carg;
  bool c, g;
};

/* function declarations */
void challenge (struct arguments *arguments);
void generate (struct arguments *arguments);
static error_t parse_opt (int key, char *arg, struct argp_state *state);

/* global variables */
const char *argp_program_version = "nsauth 1.0";
const char *argp_program_bug_address
    = "github.com/agitpropyookay/nsauth/issues";
static char doc[] = "nsauth -- cryptographic identity utility";
static char args_doc[] = "ARG";
// clang-format off
static struct argp_option options[]
    = { { "challenge", 'c', "STRING", 0, "Sign challenge",    0 },
        { "generate",  'g', 0,        0, "Generate RSA pkey", 0 },
        { 0 } };
// clang-format on
static struct argp argp = { options, parse_opt, args_doc, doc };
// ──────┬──────────┬──────────────────────────────────────────────────────────
//  bits │ time*    │ description                     last updated Sep 27 2025
// ──────┼──────────┼──────────────────────────────────────────────────────────
//   829 │ 0m0.008s │ RSA250 factored Feb 28 2020
//  1024 │ 0m0.012s │ insecure for signing as per NIST SP 800-57
//  2048 │ 0m0.040s │
//     ⋮ │ ⋮        │ recommended
//  8192 │ 0m4.126s │
// 16384 │ 1m4.129s │ max recommended by OpenSSL
// ──────┴──────────┴──────────────────────────────────────────────────────────
// * key generation user time on AMD Ryzen™ 7 9800X3D × 16
constexpr int bits = 4096; // [2048, 8192]

/* function definitions */
/**
 * handles -c, --challenge
 */
void
challenge (struct arguments *arguments)
{
  BIO *bio, *b64;
  EVP_MD_CTX *ctx = nullptr;
  bool error = true;
  FILE *fp = nullptr;
  EVP_PKEY *pkey = nullptr;
  unsigned char *sig = nullptr;
  size_t siglen;

  if (!(fp = fopen (arguments->arg, "r")))
    goto err;
  if (!(pkey = PEM_read_PrivateKey (fp, nullptr, nullptr, nullptr)))
    goto err;
  if (!PEM_write_PUBKEY (stdout, pkey))
    goto err;
  if (!(ctx = EVP_MD_CTX_create ()))
    goto err;
  if (!EVP_DigestSignInit (ctx, nullptr, EVP_sha256 (), nullptr, pkey))
    goto err;
  if (!EVP_DigestSignUpdate (ctx, arguments->carg, strlen (arguments->carg)))
    goto err;
  if (!EVP_DigestSignFinal (ctx, nullptr, &siglen))
    goto err;
  if (!(sig = OPENSSL_malloc (siglen)))
    goto err;
  if (!EVP_DigestSignFinal (ctx, sig, &siglen))
    goto err;
  puts ("-----BEGIN BASE64 SIGNATURE-----");
  b64 = BIO_new (BIO_f_base64 ());
  bio = BIO_new_fp (stdout, BIO_NOCLOSE);
  BIO_push (b64, bio);
  BIO_write (b64, sig, siglen);
  BIO_flush (b64);
  BIO_free_all (b64);
  puts ("-----END BASE64 SIGNATURE-----");
  error = false;
err:
  if (error)
    perror ("-c, --challenge failed");
  ERR_print_errors_fp (stderr);
  EVP_MD_CTX_free (ctx);
  if (fp)
    fclose (fp);
  EVP_PKEY_free (pkey);
  OPENSSL_free (sig);
}

/**
 * handles -g, --generate
 */
void
generate (struct arguments *arguments)
{
  bool error = true;
  int fd = -1;
  FILE *fp = nullptr;
  EVP_PKEY *pkey = nullptr;

  // if O_EXCL is specified in conjunction with O_CREAT, and PATHNAME already
  // exists, then open() fails with the error EEXIST
  if ((fd = open (arguments->arg, O_CREAT | O_EXCL | O_WRONLY, 0644)) == -1)
    goto err;
  if (!(fp = fdopen (fd, "w")))
    goto err;
  if (!(pkey = EVP_RSA_gen (bits)))
    goto err;
  if (!PEM_write_PrivateKey (fp, pkey, EVP_aes_256_cbc (), nullptr, 0, nullptr,
                             nullptr))
    goto err;
  error = false;
err:
  if (error)
    perror ("-g, --generate failed");
  ERR_print_errors_fp (stderr);
  fp ? fclose (fp) : close (fd);
  EVP_PKEY_free (pkey);
}

/**
 * parse a single option
 */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'c':
      arguments->c = true;
      arguments->carg = arg;
      break;
    case 'g':
      arguments->g = true;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num)
        argp_usage (state);
      arguments->arg = arg;
      break;
    case ARGP_KEY_END:
      if (!state->arg_num)
        argp_usage (state);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

int
main (int argc, char **argv)
{
  struct arguments arguments = {};

  argp_parse (&argp, argc, argv, 0, 0, &arguments);
  // calling generate() first allows both generating and using a key in one
  // invocation: $ nsa -c example -g key.pem
  if (arguments.g)
    generate (&arguments);
  if (arguments.c)
    challenge (&arguments);
  return 0;
}
