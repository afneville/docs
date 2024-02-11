---
author: Alexander Neville
date: 2024-02-11
title: GnuPG Usage
description: A guide to using the GnuPG utilitiy.
image-meta: gnupg-logo.png
image-meta-credit: https://commons.wikimedia.org/wiki/File:Gnupg_logo.svg
---

_GnuPG_, the _GNU privacy guard_, is a free and open source
implementation of the _OpenPGP_ specification. OpenPGP itself is an IETF
standard based on _Pretty Good Privacy_ (PGP).

# Configuration

It may be preferable to move the GnuPG configuration to `~/.config/`, to
avoid cluttering the home directory. GnuPG requires strict permissions
on the chosen configuration destination, removing all access permissions
from the user's group and other users.

```sh
mkdir -p ~/.config/gpg
chmod 700 ~/.config/gpg
touch ~/.config/gpg/gpg.conf
chmod 600 ~/.config/gpg/gpg.conf
export GNUPGHOME="~/.config/gpg/"
```

Here are some sensible options to place in the GnuPG configuration file.
These settings will suppress distracting information - such as version
numbers and comments - in the `gpg` output.

```text
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

When performing certain actions with GnuPG, a key passphrase is
required. The `gpg` utility delegates to a _pinentry_ program to prompt
the user for this passphrase. Many such programs exist. The preference
is set in the `gpg-agent.conf` file (in the specified configuration
directory).

```text
pinentry-program /usr/bin/pinentry-tty
```

After modifying the `gpg-agent.conf` file, reload the agent with the
command:

```text
gpg-connect-agent reloadagent /bye
```

# Create and Manage Keys

GnuPG keys can be created _quickly_ (providing only a user id and
password) or interactively (specifying the key algorithm and optionally
adding comments). Add the `--expert` option to enable newer ECC ciphers.

```text
gpg --quick-generate-key
gpg --generate-key
gpg --full-generate-key
```

View keys in the keychain (public & private respectively):

```text
gpg --list-keys
gpg --list-secret-keys
```

It is wise to export newly-generated key pairs and revocation
certificates to a safe location. The revocation certificate should be
generated when the key is created, not when it is needed. The `--armor`
flag ensures the output is ASCII encoded, rather than binary.

```text
gpg --export --armor --output pubkey.asc <uid>
gpg --export-secret-keys --armor --output seckey.asc <uid>
gpg --gen-revoke --armor --output revcert.asc <uid>
```

So that other GnuPG users are able to search for and import a public
key, publish it to a keyserver:

```text
gpg --send-keys <keyid>
gpg --seach-keys <uid>
gpg --receive-keys <keyid>
```

# Signing

To demonstrate authenticity, a user can _sign_ a file with their private
key. The distributed file can be _verified_ by a recipient with the
author's public key. If the file is modified after the signature is
generated, verification will reveal the file is not genuine. Signing a
file differs from simply appending a checksum as it guarantees the
_authenticity_ in addition to the _integrity_ of the file.

There are many different methods and options for signing a file with
GnuPG. As before, the output can be in raw binary format, or armoured
ASCII. To provide some text for this example, the first paragraph of
Charles Dickens' _A Tale of Two Cities_ is saved in the file
`message.txt`.

```text
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
signature is `.gpg`. Optionally, add the `--armour` option to encode the
output. It is always possible to encode a signature after its creation
with the `--enarmour` option. The `.asc` extension is used for such
files. In this case, the user is prompted to provide a passphrase for
the specified secret key. Credentials are cached by the `gpg-agent` and
may not be required with every `gpg` invocation that uses a secret key.

```text
$ ls
message.txt
$ gpg --default-key contact@afneville.com --sign message.txt
gpg: using "contact@afneville.com" as default secret key for signing
Please enter the passphrase to unlock the OpenPGP secret key:
"Alexander Neville <contact@afneville.com>"
255-bit EDDSA key, ID E584CD0E64E26B84,
created 2023-10-03.

Passphrase:
$ ls
message.txt  message.txt.gpg
```

The file created by `--sign` is raw, but not encrypted. It can be
verified by any recipient with the author's public key. The original
file can be viewed with the `--decrypt` option.

```text
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
gpg: Signature made Sun 11 Feb 2024 20:50:03 GMT
gpg:                using EDDSA key BB302E2E1E8460173DD869A0E584CD0E64E26B84
gpg:                issuer "contact@afneville.com"
gpg: Good signature from "Alexander Neville <contact@afneville.com>" [ultimate]
Primary key fingerprint: BB30 2E2E 1E84 6017 3DD8  69A0 E584 CD0E 64E2 6B84
```

## Clearsign & Detached Signatures

In some cases, an individual may want to provide a signature against
which recipients can optionally verify the message, but leaving the
original message in clear/plain text. This can be done with the
`--clearsign` option, which appends the ASCII armoured signature to the
end of the plain text file (notice the header appended to the start of
the file).

```text
$ gpg --default-key contact@afneville.com --clearsign message.txt
gpg: using "contact@afneville.com" as default secret key for signing
$ cat message.txt.asc
-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

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

iIwEARYKADQWIQS7MC4uHoRgFz3YaaDlhM0OZOJrhAUCZck5xBYcY29udGFjdEBh
Zm5ldmlsbGUuY29tAAoJEOWEzQ5k4muEf58BAMY1Z6Y/VFu4xAwa3hrny3YTgKBk
DiTExdMuT6QWTaWPAP441yDMhWsgJIoAcIDBx4zLNEK6UOgeEnpnMuwsJLc6DQ==
=tnDp
-----END PGP SIGNATURE-----
```

This file can be verified as before. In another common scenario, an
individual may want to sign a binary file before distributing it. GPG
can create a _detached_ signature to distribute alongside the original
file that does not include the file contents. The default file extension
for a detached signature is `.sig`. The armoured detached signature is
identical to the signature added to the end of the clearsigned file.

```text
$ gpg --default-key contact@afneville.com --detach-sign message.txt
gpg: using "contact@afneville.com" as default secret key for signing
$ gpg --default-key contact@afneville.com --detach-sign --armor message.txt
gpg: using "contact@afneville.com" as default secret key for signing
$ ls
message.txt  message.txt.asc  message.txt.sig
$ cat message.txt.asc
-----BEGIN PGP SIGNATURE-----

iIwEABYKADQWIQS7MC4uHoRgFz3YaaDlhM0OZOJrhAUCZck6lhYcY29udGFjdEBh
Zm5ldmlsbGUuY29tAAoJEOWEzQ5k4muEwwsA/3DVBxptIjoErllC7WE6Pv7QpsZ1
XrAQd8Ye6BnmkluQAQCvcJWEzY2lqtduSgzvjrYsgWlDSC4JtohQeMvMqaPOAA==
=OR1+
-----END PGP SIGNATURE-----
```

When verifying a detached signature, GPG will assume the original file
is stored with the same name as the signature, less the `.asc` / `.sig`
suffix. The path to the original file can be given explicitly by passing
it to `gpg` after the signature path.

```text
$ gpg --verify message.txt.sig
gpg: assuming signed data in 'message.txt'
gpg: Signature made Sun 11 Feb 2024 21:22:11 GMT
gpg:                using EDDSA key BB302E2E1E8460173DD869A0E584CD0E64E26B84
gpg:                issuer "contact@afneville.com"
gpg: Good signature from "Alexander Neville <contact@afneville.com>" [ultimate]
Primary key fingerprint: BB30 2E2E 1E84 6017 3DD8  69A0 E584 CD0E 64E2 6B84
$ gpg --verify message.txt.asc message.txt
gpg: Signature made Sun 11 Feb 2024 21:22:30 GMT
gpg:                using EDDSA key BB302E2E1E8460173DD869A0E584CD0E64E26B84
gpg:                issuer "contact@afneville.com"
gpg: Good signature from "Alexander Neville <contact@afneville.com>" [ultimate]
Primary key fingerprint: BB30 2E2E 1E84 6017 3DD8  69A0 E584 CD0E 64E2 6B84
```

# Symmetric Encryption

Unlike other GnuPG operations, symmetric encryption does not require a
key pair. The supplied passphrase is the only secret/seed in the
encryption process.

```language-plaintext
$ gpg -c message.txt
Enter passphrase

Passphrase:
Repeat:
$
$ ls
message.txt  message.txt.gpg
```

To decrypt an encrypted file, use the `--decrypt` option.

# Asymmetric Encryption

Asymmetric encryption is widely used to secure internet communications.
The public key of any key pair may be freely disseminated, so that
anyone can use it to begin an encrypted conversation. The recipient's
public key is used to perform encryption. Once encrypted, only the
intended recipient is able to decrypt the message, using their private
key.

```text
$ gpg --recipient git@afneville.com --encrypt message.txt
$ ls
message.txt  message.txt.gpg
$ gpg --decrypt message.txt.gpg
gpg: encrypted with cv25519 key, ID E3E3546CAA066A41, created 2023-10-03
      "Alexander Neville <git@afneville.com>"
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

GnuPG allows a sender to sign their message before it is encrypted. The
recipient is then able to verify that the decrypted message was authored
by the sender.

```text
$ gpg --default-key contact@afneville.com --recipient git@afneville.com --encrypt --sign message.txt
gpg: using "contact@afneville.com" as default secret key for signing
$ ls
message.txt  message.txt.gpg
$ gpg --decrypt message.txt.gpg
gpg: encrypted with cv25519 key, ID E3E3546CAA066A41, created 2023-10-03
      "Alexander Neville <git@afneville.com>"
It was the best of times, it was the worst of times, it was the age of
wisdom, it was the age of foolishness, it was the epoch of belief, it
was the epoch of incredulity, it was the season of Light, it was the
season of Darkness, it was the spring of hope, it was the winter of
despair, we had everything before us, we had nothing before us, we were
all going direct to Heaven, we were all going direct the other way--in
short, the period was so far like the present period, that some of its
noisiest authorities insisted on its being received, for good or for
evil, in the superlative degree of comparison only.
gpg: Signature made Sun 11 Feb 2024 21:35:35 GMT
gpg:                using EDDSA key BB302E2E1E8460173DD869A0E584CD0E64E26B84
gpg:                issuer "contact@afneville.com"
gpg: Good signature from "Alexander Neville <contact@afneville.com>" [ultimate]
Primary key fingerprint: BB30 2E2E 1E84 6017 3DD8  69A0 E584 CD0E 64E2 6B84
```

In these examples, the _trust_ level of the keys is _ultimate_ as they
were both created by the author. By default, imported keys will not have
this level of trust.
