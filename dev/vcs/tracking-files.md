---
author: Alexander Neville
date: 2023-02-23
title: Tracking Files with Git
description:
  Staging files and creating commits with git.
image-meta: git-logo.png
image-meta-credit: https://git-scm.com/downloads/logos
---

Git exposes a set of commands for checking the status of the repository
and files within it. Git also includes an intuitive interface for moving
files between the different states. The same can be achieved using more
primitive operations, but all modern git versions share a very similar
CLI.

The `git status` command outputs information relevant to the current
state of the repository. In a recently initialised, empty project the
output is very short.

```text
$ git status
On branch main

No commits yet

nothing to commit (create/copy files and use "git add" to track)
```

# Untracked Files

A file which was not in the previous snapshot is said to be _untracked_,
git has no version history for it. The output of the status command
reports that `git add` will move an untracked file to the staging area,
from where it can be committed. After these two operations, the working
directory is once again a direct check out of the git directory.

```text
$ echo "README" > README.md
$ git status
On branch main

No commits yet

Untracked files:
(use "git add <file>..." to include in what will be committed)
README.md

nothing added to commit but untracked files present (use "git add" to track)
$ git add README.md
$ git commit -m "add README.md" # -m flag specifies the commit message
[main (root-commit) 8f570f8] add README.md
1 file changed, 1 insertion(+)
create mode 100644 README.md
$ git status
On branch main
nothing to commit, working tree clean
```

The `-m` flag is used to provide a commit message. Committing without
the `-m` flag will open the user's preferred editor to enter a message.
The `-a` flag can be used to stage and commit all changes to the working
directory, effectively bypassing the staging area.

New untracked files and changes to tracked files are shown by
`git status`.

```text
$ echo "int main() { return 0; }" > main.c
$ echo "# README.md" > README.md
$ git status
On branch main
Changes not staged for commit:
(use "git add <file>..." to update what will be committed)
(use "git restore <file>..." to discard changes in working directory)
modified:   README.md

Untracked files:
(use "git add <file>..." to include in what will be committed)
main.c

no changes added to commit (use "git add" and/or "git commit -a")
```

By _adding_ all the files in the working directory, tracked and
untracked files alike become staged.

```text
$ git add *
$ git status
On branch main
Changes to be committed:
(use "git restore --staged <file>..." to unstage)
modified:   README.md
new file:   main.c
$ git commit -m "add main.c and fix README.md syntax"
[main 34250c7] add main.c and fix README.md syntax
2 files changed, 2 insertions(+), 1 deletion(-)
create mode 100644 main.c
$ git status
On branch main
nothing to commit, working tree clean
```

# Modifying Files

In this scenario, a committed file is modified in the working directory;
the change is staged and then the same file is modified a second time in
the working directory.

```text
$ echo "git is a vcs" >> README.md
$ git add README.md
$ echo "this line isn't staged" >> README.md
$ git status
On branch main
Changes to be committed:
(use "git restore --staged <file>..." to unstage)
modified:   README.md

Changes not staged for commit:
(use "git add <file>..." to update what will be committed)
(use "git restore <file>..." to discard changes in working directory)
modified:   README.md
```

It seems from the output of the status command that the file is in two
states. This demonstrates what happens when a file is staged: a snapshot
of the file at the point it was staged is earmarked for inclusion in the
next commit. Subsequent changes to the same file are not reflected in
the snapshot to be committed.

# Status and Diff

The short form of the `git status` command is a concise and helpful way
of visualising these states. Create some changes to the repository and
then use `git status --short` or `git status -s`:

```text
$ vi main.c
$ vi main.py
$ vi LICENSE.txt
$ $EDITOR
$ git status
On branch main
Changes to be committed:
(use "git restore --staged <file>..." to unstage)
modified:   README.md
new file:   main.py

Changes not staged for commit:
(use "git add <file>..." to update what will be committed)
(use "git restore <file>..." to discard changes in working directory)
modified:   README.md
modified:   main.c

Untracked files:
(use "git add <file>..." to include in what will be committed)
LICENSE.txt
$ git status -s
MM README.md
M main.c
A  main.py
?? LICENSE.txt
```

The normal status command shows the untracked, modified and staged files
as before. The short form output shows all the changes to the working
directory and two columns for each file. The leftmost column represents
the staging area and the second column represents the working directory.
The meaning of these letters:

- `M`: modified
- `A`: added (previously untracked)
- `??`: untracked
- `D`: deleted

For a more granular view of changes to the repository there is the
command `git diff`. By default, `git diff` shows the differences between
the working directory and the index for every modified file. The
`--cached` and `--staged` flags allow the index to be compared with the
last commit. Optionally a path can be supplied to view the differences
for a single file.

```text
$ git diff main.c
diff --git a/main.c b/main.c
index 76e8197..40cbb54 100644
--- a/main.c
+++ b/main.c
@@ -1 +1 @@
-int main() { return 0; }
+int main() { return 1; }
$ git diff --cached
diff --git a/README.md b/README.md
index 7e59600..5d937d3 100644
--- a/README.md
+++ b/README.md
@@ -1 +1,2 @@
## README
+git is a vcs
diff --git a/main.py b/main.py
new file mode 100644
index 0000000..cd9ac48
--- /dev/null
+++ b/main.py
@@ -0,0 +1,6 @@
+def main():
+    pass
+
+
+if __name__ == "__main__":
+    main()
```

# Removing Files

Removing a file in the conventional way does not removed it from the git
database or even the index; the removed file is not staged for commit.
The deletion can, however, be staged in the usual way.

```text
$ rm file1
$ git status
On branch main
Changes not staged for commit:
(use "git add/rm <file>..." to update what will be committed)
(use "git restore <file>..." to discard changes in working directory)
deleted:    file1
$ git add file1
$ git status
no changes added to commit (use "git add" and/or "git commit -a")
On branch main
Changes to be committed:
(use "git restore --staged <file>..." to unstage)
deleted:    file1
```

The `git rm` command removes a file (or files) from the working
directory and updates the staging area to reflect the change.

```text
$ git rm file2
rm 'file2'
$ gs
On branch main
Changes to be committed:
(use "git restore --staged <file>..." to unstage)
deleted:    file2
```
