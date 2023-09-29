---
title: Remotes Repositories & Branches
author: Alexander Neville
date: 2023-09-26
---

In a collaborative setting, remote repositories allow multiple
developers to share work. One project may have more than one remote.

# Managing Remotes

The remote repositories for the current project can be manipulated with
the `git remote` command. Without any optional arguments, all remotes
are listed. Inspect `man git-remote` to view the available subcommands.

```
$ git remote
origin
```

By default, git will name the server that a repository is cloned from
_origin_. To inspect this remote, use the `show` subcommand

```
$ git remote show origin
* remote origin
  Fetch URL: git@github.com:alexanderneville/docs.git
  Push  URL: git@github.com:alexanderneville/docs.git
  HEAD branch: main
  Remote branches:
    main tracked
    org  tracked
  Local branch configured for 'git pull':
    main merges with remote main
  Local ref configured for 'git push':
    main pushes to main (up to date)
```

## Remote Branches

A remote branch exists on some remote server. If a remote is configured
for a project, git maintains local _remote-tracking branches_. Unlike
conventional branches, the user cannot manipulate remote-tracking
branches directly. Git maintains these branches to accurately represent
the state of the remote repository at the point it was last checked.
Remote-tracking branches are of the form `remote/branch`.

# Branch Management

The `branch` subcommand with no optional arguments prints a list of
local branches. The `-r` flag can be used to list remote-tracking
branches and `-a` can be used to list both local and remote-tracking
branches.

```
$ git branch
* main
$ git branch -r
  origin/HEAD -> origin/main
  origin/main
  origin/org
$ git branch -a
* main
  remotes/origin/HEAD -> origin/main
  remotes/origin/main
  remotes/origin/org
```

# Tracking Branches

Fetching or cloning a remote repository populates the working directory
with remote-tracking branches, but it does not create an editable local
branch for each. Checking out a remote-tracking branch will put the
local repository in detached head state.

```
$ git checkout origin/org
Note: switching to 'origin/org'.

You are in 'detached HEAD' state. You can look around, make experimental
changes and commit them, and you can discard any commits you make in this
state without impacting any branches by switching back to a branch.
```

To _checkout_ a useful copy of a remote-tracking branch, a local branch
must be created. Any local branch checked out from a remote-tracking
branch is known as a _tracking branch_. The remote branch it tracks is
known as the _upstream_ branch. The syntax to do so is
`git branch <local-branch> <remote>/<branch>`. This command won't switch
to the newly created branch, but `git checkout -b ...` will.

```
$ git checkout -b test origin/org
branch 'test' set up to track 'origin/org'.
Switched to a new branch 'test'
$ git branch -vv
  main d410267 [origin/main] restructure
* test ba1398c [origin/org] change file links
```

This is a common operation and consequently has multiple shorthand
forms. The `--track` option of `git checkout` will create a local branch
with the same name of some remote branch, given a remote-tracking branch
as an argument. If the checkout subcommand is passed the name of a
non-existent branch that matches exactly one remote branch (across all
configured remotes), it will create a tracking branch with that name.

```
$ git checkout org
branch 'org' set up to track 'origin/org'.
Switched to a new branch 'org'
$ git branch -vv
  main d410267 [origin/main] restructure
* org  ba1398c [origin/org] change file links
```

To do the opposite and set an existing local branch to track a remote
branch or change the remote branch being tracked, use the `-u` option of
`git branch` to set the upstream branch.

```
$ git branch -vv
* main d410267 [origin/main] restructure
$ git branch -u origin/org
branch 'main' set up to track 'origin/org'.
$ git branch -vv
* main d410267 [origin/org: ahead 50] restructure
```

Git provides some useful flags for identifying which branches can be
safely deleted. `git branch --merged` lists only the branches which are
fully included by the specified commit or `HEAD` by default, i.e.
branches which are an ancestor of the specified commit. The
`--no-merged` option does the opposite, listing only branches which are
not direct ancestors of the current commit.

```
$ git branch --merged
* main
  org
$ git branch --no-merged=HEAD
$ git branch --no-merged=org
* main
$ git branch --merged=org
  org
```

If a topic branch is no longer useful, it can be deleted with
`git branch -d <branch-name>`. To delete a remote branch, use the
command `git push <remote> --delete <branch>`.
