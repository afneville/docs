---
author: Alexander Neville
date: 2024-02-02
title: Git Revision Selection
description: Selecting git objects with revision notation.
image-meta: git-logo.png
image-meta-credit: https://git-scm.com/downloads/logos
---

A git _revision_ usually denotes a commit or, in rare cases, a git
object of a different type. Revision notation can be used to select one
or a number of commits, in addition to blobs, trees and other objects.
Revisions and revision ranges are often found amongst the parameters of
git commands. Complete revision reference resides in the
`gitrevisions(7)` page of the system manuals.

# SHA-1 syntax

Commits can be identified with an unabridged 40 character long SHA-1
hash, or a shorter unambiguous abbreviation of four or more characters.
Passing the `--abbrev-commit` option to `git log` will print the hashes
in seven characters by default with as few additional characters as
required to uniquely identify a commit.

```text
$ git show --shortstat 73e358ebd24810fe7055bf952d882c34d68a9805
commit 73e358ebd24810fe7055bf952d882c34d68a9805
Author: Alexander Neville <git@afneville.com>
Date:   Sun Sep 3 22:16:14 2023 +0100

    promote all headings

 29 files changed, 350 insertions(+), 327 deletions(-)
$ git show --shortstat 73e358e
commit 73e358ebd24810fe7055bf952d882c34d68a9805
Author: Alexander Neville <git@afneville.com>
Date:   Sun Sep 3 22:16:14 2023 +0100

    promote all headings

 29 files changed, 350 insertions(+), 327 deletions(-)
```

# Ancestry References

It is possible to identify a commit relatively from some reference,
using the `~` and `^` characters. Individually, both refer to the
specified commit's parent. When the caret is followed by a number, it
denotes which parent of the current commit to select in the case of
merge commits. When the tilde is followed by a number it represents the
number of times to get the first parent of the selected commit.

```text
$ git log --graph --oneline
* ba6fe2a (HEAD -> main) modify file3
*   678bca1 Include new files
|\
| * 6c505f1 (topic1) add file3
| * 6e367d6 add file2
* | 757bf51 modify file1
|/
* 8b96417 initial commit
```

`HEAD^` and `HEAD~` clearly refer to the same commit.

```text
$ git show --oneline HEAD~
678bca1 Include new files
$ git show --oneline HEAD^
678bca1 Include new files
```

The tilde and the caret select the same commit when followed by a `1`,
though this is coincidental.

```text
$ git show --oneline --no-patch --no-decorate 'HEAD^^1'
757bf51 modify file1
$ git show --oneline --no-patch --no-decorate 'HEAD~~1'
757bf51 modify file1
```

The difference is apparent when a larger number is chosen. `^2` selects
the second of two ancestors, while `~2` selects the first ancestor of
the commit twice.

```text
$ git show --oneline --no-patch --no-decorate 'HEAD^^2'
6c505f1 add file3
$ git show --oneline --no-patch --no-decorate 'HEAD~~2'
8b96417 initial commit
```

## Commit Ranges

The double dot syntax is used to list commits reachable via one
reference, but not the other. The first of the two examples below lists
the ancestors of the `topic1` branch that are not ancestors of the first
ancestor of the merge commit.

```text
$ git log --oneline --no-decorate 'HEAD~^1..topic1'
6c505f1 add file3
6e367d6 add file2
$ git log --oneline --no-decorate 'topic1..HEAD~^1'
757bf51 modify file1
```

# Reflog

Git maintains a local _history_ of sorts. This history, called the
_reflog_, works a little like a shell's history; the reflog is local and
temporary, rather than a persistent part of the project's history or its
configuration. The `git reflog` command shows the history of the branch
tip. There is an entry for every occasion on which it was moved.

```text
$ git reflog
d410267 (HEAD -> main, origin/main, origin/HEAD) HEAD@{0}: checkout: moving from ba1398c98cf0f2db6adc58d81b5d146136b556da to main
ba1398c (origin/org) HEAD@{1}: checkout: moving from main to origin/org
d410267 (HEAD -> main, origin/main, origin/HEAD) HEAD@{2}: reset: moving to HEAD
d410267 (HEAD -> main, origin/main, origin/HEAD) HEAD@{3}: commit: restructure
73e358e HEAD@{4}: commit: promote all headings
8deef80 HEAD@{5}: commit: alexneville.co.uk -> afneville.com
001e5ff HEAD@{6}: commit (amend): fix link target on index page
8a1de37 HEAD@{7}: commit: fix link target on index page
54db34c HEAD@{8}: rebase (finish): returning to refs/heads/main
54db34c HEAD@{9}: rebase (pick): webkit fix
b295712 HEAD@{10}: rebase (reword): hide menu on scroll
c14b67a HEAD@{11}: rebase (reword): hide menu on scoll
bc84c86 HEAD@{12}: rebase (pick): update footer
8159492 HEAD@{13}: rebase (fixup): Complete article on C design patterns.
d839f39 HEAD@{14}: rebase (start): checkout HEAD~5
...
```

The position of the `HEAD` pointer as it was before being moved four
times to date can be checked with the `HEAD@{4}` syntax.

```text
$ git show --shortstat HEAD@{4}
commit 73e358ebd24810fe7055bf952d882c34d68a9805
Author: Alexander Neville <git@afneville.com>
Date:   Sun Sep 3 22:16:14 2023 +0100

    promote all headings

 29 files changed, 350 insertions(+), 327 deletions(-)
```

A local reflog is maintained for other branches. The state of the branch
at a certain point in time is a valid query.

```text
$ git show --shortstat main@{2.months.ago}
commit 56b0d14ecce0f8572adf1fa6faed808aec18aa23
Author: Alexander Neville <git@afneville.com>
Date:   Thu Jul 20 14:56:32 2023 +0100

    Correct errors in src and res files

 11 files changed, 75 insertions(+), 224 deletions(-)
```

For more information and examples, consult the manual pages for
`git-reflog(1)`.

# Tags

Create a lightweight tag without an annotation:

```text
$ git tag v1.0
$ git tag
v1.0
$ git show v1.0
commit 2c35a17e92118e9c8b160438d0270af72f289ce3 (HEAD -> main, tag: v1.0)
Author: Alexander Neville <git@afneville.com>
Date:   Tue Mar 28 15:38:42 2023 +0100

    create README
```

Delete a tag:

```text
$ git tag
v1.0
$ git tag -d v1.0
Deleted tag 'v1.0' (was 2c35a17)
$ git tag
```

Create an annotated tag:

```text
$ git tag
$ git tag -a v1.1 -m "version 1.1 with CONTRIBUTING info"
$ git tag
v1.1
$ git show v1.1
tag v1.1
Tagger: Alexander Neville <git@afneville.com>
Date:   Tue Mar 28 16:02:58 2023 +0100

version 1.1 with CONTRIBUTING info

commit f47ea70f45dc677673556040d3d2f0fc5ac452d0 (HEAD -> main, tag: v1.1)
Author: Alexander Neville <git@afneville.com>
Date:   Tue Mar 28 15:57:50 2023 +0100

    create CONTRIBUTING
```

Create a tag for a previous commit:

```text
$ git log --oneline
f47ea70 (HEAD -> main, tag: v1.1) create CONTRIBUTING
2c35a17 create README
$ git tag v1.0 2c35a17
$ git tag
v1.0
v1.1
$ git log --oneline
f47ea70 (HEAD -> main, tag: v1.1) create CONTRIBUTING
2c35a17 (tag: v1.0) create README
```
