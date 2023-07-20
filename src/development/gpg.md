---
author: Alex Neville
date: 2023-06-11
title: GPG Usage
---

_GnuPG_, the _GNU privacy guard_ is a free and open source
implementation of the _OpenPGP_ specification. OpenPGP itself is an IETF
standard based on _Pretty Good Privacy_ (PGP).

## Configuration

You may want to move the GPG configuration to `~/.config/`, to avoid
clutter in your home directory. GPG requires strict permissions on the
chosen configuration destination, removing all access to the user's
group and all other users.

```sh
mkdir -p ~/.config/gpg
chmod 700 ~/.config/gpg
touch ~/.config/gpg/gpg.conf
chmod 600 ~/.config/gpg/gpg.conf
export GNUPGHOME="~/.config/gpg/"
```

Here are some sensible options to place in the `gpg` configuration file.
These settings will suppress distracting information - such as version
numbers and comments - in the `gpg` output.

```{.language-plaintext}
no-default-keyring
keyserver-options auto-key-retrieve
keyserver-options no-honor-keyserver-url
auto-key-locate keyserver hkps://keys.openpgp.org
keyserver hkps://keys.openpgp.org
keyid-format LONG
with-fingerprint
with-keygrip
no-emit-version
no-comments
no-greeting
```

When performing certain actions with `gpg`, the key passphrase is
required. The `gpg` utility delegates to a _pinentry_ program to prompt
for this passphrase. Many such programs exist. The preference is set in
the `gpg-agent.conf` file.

```{.language-plaintext}
pinentry-program /usr/bin/pinentry-tty
```

After modifying the `gpg-agent.conf` file, reload the agent with the
command `gpg-connect-agent reloadagent /bye`.

## Create and Manage Keys

GPG keys can be created quickly, providing only a user id and password
or interactively, specifying the key algorithm and optionally adding
comments. Add the `--expert` option to enable newer ECC ciphers.

```sh {.language-plaintext}
gpg --quick-generate-key
gpg --generate-key
gpg --full-generate-key
```

View keys in the keychain (public & private respectively).

```sh {.language-plaintext}
gpg --list-keys
gpg --list-secret-keys
```

It is wise to export your key pairs and revocation certificate to a safe
location. The revocation certificate should be generated when the key is
created, not when it is needed. The `--armor` flag ensures the output is
ascii encoded, rather than binary.

```sh {.language-plaintext}
gpg --export --armor --output pubkey.asc uid
gpg --export-secret-keys --armor --output seckey.asc uid
gpg --gen-revoke --armor --output revcert.asc uid
```

So that other _gpg_ users are able to search for and import your public
key, publish it to a keyserver.

```sh {.language-plaintext}
gpg --send-keys keyid
gpg --seach-keys uid
gpg --receive-keys keyid
```

## Signing

To demonstrate authenticity, a user can _sign_ a file with their private
key. The distributed file can be _verified_ by a recipient with the
author's public key. If the file is modified after the signature is
generated, verification will reveal the file is not genuine. Signing a
file differs from a checksum as it guarantees authenticity in addition
to integrity.

There are many different methods for signing a file with `gpg`. As
before, the output can be in raw binary format, or armoured ASCII. For
this example, the first paragraph of Charles Dickens' _"A Tale of Two
Cities"_ is saved in the file `message.txt`.

```{.language-plaintext}
It was the best of times, it was the worst of times, it was the age of
wisdom, it was the age of foolishness, it was the epoch of belief, it
was the epoch of incredulity, it was the season of Light, it was the
season of Darkness, it was the spring of hope, it was the winter of
despair, we had everything before us, we had nothing before us, we were
all going direct to Heaven, we were all going direct the other way--in
short, the period was so far like the present period, that some of its
noisiest authorities insisted on its being received, for good or for
evil, in the superlative degree of comparison only.
```

The default `--sign` command will compress and store the file contents
alongside the signature in raw format. The default extension for such a
signature is `.gpg`. Optionally add the `--armour` option to encode the
output. It is always possible to encode a signature later with
`gpg --enarmour`. The `.asc` extension is used for such files.

```{.language-plaintext}
$ ls
message.txt
$ gpg --default-key contact@alexneville.co.uk --sign message.txt
gpg: using "contact@alexneville.co.uk" as default secret key for signing
Please enter the passphrase to unlock the OpenPGP secret key:
"Alex Neville <contact@alexneville.co.uk>"
255-bit EDDSA key, ID 97BAC3EFF6C6C53D,
created 2023-06-11.

Passphrase:
$ ls
message.txt  message.txt.gpg
```

The file created by `--sign` is raw, but not encrypted. It can be
verified by any recipient with the author's public key. The original
file can be viewed with the `--decrypt` command.

```{.language-plaintext}
$ gpg --decrypt message.txt.gpg
It was the best of times, it was the worst of times, it was the age of
wisdom, it was the age of foolishness, it was the epoch of belief, it
was the epoch of incredulity, it was the season of Light, it was the
season of Darkness, it was the spring of hope, it was the winter of
despair, we had everything before us, we had nothing before us, we were
all going direct to Heaven, we were all going direct the other way--in
short, the period was so far like the present period, that some of its
noisiest authorities insisted on its being received, for good or for
evil, in the superlative degree of comparison only.
gpg: Signature made Thu 20 Jul 2023 10:25:16 BST
gpg:                using EDDSA key 6526651FCB32C82BB3A6449C97BAC3EFF6C6C53D
gpg:                issuer "contact@alexneville.co.uk"
gpg: Good signature from "Alex Neville <contact@alexneville.co.uk>" [ultimate]
Primary key fingerprint: 6526 651F CB32 C82B B3A6  449C 97BA C3EF F6C6 C53D
```

### Clearsign & Detached Signatures

In some cases an individual may want to provide a signature against
which recipients can optionally verify the message, but leaving the
original message in plaintext. This can be done with `--clearsign`,
which appends the ASCII armoured signature to the end of the plain text
file (notice the header appended to the start of the file).

```{.language-plaintext}
$ gpg --default-key contact@alexneville.co.uk --clearsign message.txt
gpg: using "contact@alexneville.co.uk" as default secret key for signing
$ cat message.txt.asc
-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

It was the best of times, it was the worst of times, it was the age of
wisdom, it was the age of foolishness, it was the epoch of belief, it
was the epoch of incredulity, it was the season of Light, it was the
season of Darkness, it was the spring of hope, it was the winter of
despair, we had everything before us, we had nothing before us, we were
all going direct to Heaven, we were all going direct the other way--in
short, the period was so far like the present period, that some of its
noisiest authorities insisted on its being received, for good or for
evil, in the superlative degree of comparison only.
-----BEGIN PGP SIGNATURE-----

iJAEARYIADgWIQRlJmUfyzLIK7OmRJyXusPv9sbFPQUCZLj+YBocY29udGFjdEBh
bGV4bmV2aWxsZS5jby51awAKCRCXusPv9sbFPW0yAP91qewLq+zSkrDfzhPqPMnw
oupvvG69Cxj6tjr2eWZs2gEAkStCpZE4ZgtQ9Y9Zq/WgjHM+T9zxB88hTYXfDH93
EA0=
=go6o
-----END PGP SIGNATURE-----
```

This file can be verified as before. In another common scenario, an
individual may want to sign a binary file before distributing it. GPG
can create a _detached_ signature to distribute alongside the original
file that does not include the file contents. The default file extension
for a detached signature is `.sig`. The armoured detached signature is
identical to the signature added to the end of the clearsigned file.

```{.language-plaintext}
$ gpg --default-key contact@alexneville.co.uk --detach-sign message.txt
$ ls
message.txt message.txt.sig
$ gpg --default-key contact@alexneville.co.uk --detach-sign --armour message.txt
$ ls
message.txt  message.txt.asc  message.txt.sig
gpg: using "contact@alexneville.co.uk" as default secret key for signing
$ cat message.txt.asc
-----BEGIN PGP SIGNATURE-----

iJAEABYIADgWIQRlJmUfyzLIK7OmRJyXusPv9sbFPQUCZLj+uBocY29udGFjdEBh
bGV4bmV2aWxsZS5jby51awAKCRCXusPv9sbFPaBFAP9QR+gy4YyNinSOabQzXmad
GvSjEl5gW2sGx5vVGx0DmwEA9zUgxBLg4MyDPhAggjZD7cawjq565JHomVo5DmDr
VwU=
=ullr
-----END PGP SIGNATURE-----
```

When verifying a detached signature, GPG will assume the original file
is stored with the same name as the signature, less the `.asc` / `.sig`
suffix. The path to the original file can be given explicitly by passing
it to `gpg` after the signature path.

```{.language-plaintext}
$ gpg --verify message.txt.sig
gpg: assuming signed data in 'message.txt'
gpg: Signature made Thu 20 Jul 2023 10:50:07 BST
gpg:                using EDDSA key 6526651FCB32C82BB3A6449C97BAC3EFF6C6C53D
gpg:                issuer "contact@alexneville.co.uk"
gpg: Good signature from "Alex Neville <contact@alexneville.co.uk>" [ultimate]
Primary key fingerprint: 6526 651F CB32 C82B B3A6  449C 97BA C3EF F6C6 C53D
$ gpg --verify message.txt.sig message.txt
gpg: Signature made Thu 20 Jul 2023 10:50:07 BST
gpg:                using EDDSA key 6526651FCB32C82BB3A6449C97BAC3EFF6C6C53D
gpg:                issuer "contact@alexneville.co.uk"
gpg: Good signature from "Alex Neville <contact@alexneville.co.uk>" [ultimate]
Primary key fingerprint: 6526 651F CB32 C82B B3A6  449C 97BA C3EF F6C6 C53D
```

## Symmetric Encryption

Unlike other GPG operations, symmetric encryption does not require a key
pair. The supplied passphrase is the only secret/seed in the encryption
process.

```{.language-plaintext}
$ gpg -c message.txt
Enter passphrase

Passphrase:
Repeat:
$
$ ls
message.txt  message.txt.gpg
```

To decrypt an encrypted file, use the `--decrypt` command.

## Asymmetric Encryption

Asymmetric encryption is widely used to secure internet communications.
The public key of any key pair may be freely disseminated, so that
anyone can use it to begin an encrypted conversation. The recipient's
public key is used to perform encryption. Once encrypted, only the
intended recipient is able to decrypt the message, using their private
key. I'll use the public key of another of my public email addresses for
this example.

```{.language-plaintext}
$ gpg --recipient dev@alexneville.co.uk --encrypt message.txt
$ ls
message.txt  message.txt.gpg
$ gpg --decrypt message.txt.gpg
Please enter the passphrase to unlock the OpenPGP secret key:
"Alex Neville <dev@alexneville.co.uk>"
255-bit ECDH key, ID C0E91A549735460F,
created 2023-06-11 (main key ID 545E6C5AC1F2BFE1).

Passphrase:
gpg: encrypted with 255-bit ECDH key, ID C0E91A549735460F, created 2023-06-11
      "Alex Neville <dev@alexneville.co.uk>"
It was the best of times, it was the worst of times, it was the age of
wisdom, it was the age of foolishness, it was the epoch of belief, it
was the epoch of incredulity, it was the season of Light, it was the
season of Darkness, it was the spring of hope, it was the winter of
despair, we had everything before us, we had nothing before us, we were
all going direct to Heaven, we were all going direct the other way--in
short, the period was so far like the present period, that some of its
noisiest authorities insisted on its being received, for good or for
evil, in the superlative degree of comparison only.
```

GPG allows a sender to sign their message before it is encrypted. The
recipient is then able to verify that the decrypted message was authored
by the sender. Using two of my own key pairs, I can send myself an
encrypted message - very secure!

```{.language-plaintext}
$ gpg --default-key contact@alexneville.co.uk --recipient dev@alexneville.co.uk --encrypt --sign message.txt
gpg: using "contact@alexneville.co.uk" as default secret key for signing
$ ls
message.txt  message.txt.gpg
$ gpg --decrypt message.txt.gpg
gpg: encrypted with 255-bit ECDH key, ID C0E91A549735460F, created 2023-06-11
      "Alex Neville <dev@alexneville.co.uk>"
It was the best of times, it was the worst of times, it was the age of
wisdom, it was the age of foolishness, it was the epoch of belief, it
was the epoch of incredulity, it was the season of Light, it was the
season of Darkness, it was the spring of hope, it was the winter of
despair, we had everything before us, we had nothing before us, we were
all going direct to Heaven, we were all going direct the other way--in
short, the period was so far like the present period, that some of its
noisiest authorities insisted on its being received, for good or for
evil, in the superlative degree of comparison only.
gpg: Signature made Thu 20 Jul 2023 13:50:23 BST
gpg:                using EDDSA key 6526651FCB32C82BB3A6449C97BAC3EFF6C6C53D
gpg:                issuer "contact@alexneville.co.uk"
gpg: Good signature from "Alex Neville <contact@alexneville.co.uk>" [ultimate]
Primary key fingerprint: 6526 651F CB32 C82B B3A6  449C 97BA C3EF F6C6 C53D
```
