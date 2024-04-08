# Excercise 1: Rebase

This excercise should familiarize you with the multiple uses of the `rebase` command, as we have seen in the lectures.

The excerise will ask you to explore the Daisyworld repository and familizarize yourself with its content, in particular the `display` branch.

Afterward, you will use `rebase` to clean up the history of the branch, before merging it back into the `main` branch.

Optionally, you can try using `rebase` in conjunction with `reset` to split a commit.

## Explore the repository

Take some time to explore the repository and it's commits. If you want you can try to understand what the code does but it's not necessary for the excercise.

List the branch that exist in the repository.

> <details><summary>Reveal the commands</summary>
>
> `git branch --list` (or just `git branch`) will list all the branches that are present in the local repository.
>
>Since we just cloned the repository, some branches are still only on the remote repository. You can see remote branches with the `--remote` or all branches with
>
>```bash
>git branch --all
>```
>
></details>

Explore the history of the repository, seeing the metadata of the commits, both in the `main` branch and in the `display` branch.

><details><summary>Reveal the commands</summary>
>
>To explore the history of the repository you can you the `git log` command.
>
>To change to the `display` branch you can use:
>
>```bash
>git switch display
>```
>
>This branch doesn't exists locally, but the name matches a remote branch. Git will behave as expected:
>
>```text
>Branch 'display' set up to track remote branch 'display' from 'origin'.
>Switched to a new branch 'display'
>```
>
></details>

Can you find out where the `display` branch first branched from main?

><details><summary>Reveal solution</summary>
>
>The two branches diverged after the commit:
>
>```text
>* 81ba2ab Add daisies spreading step
>```
>
>You can find this out by comparing the commit history of the two branches.
>
>A visual way to do it is with:
>
>```bash
>git log --graph --oneline --all
>```
>
>Which will print something similar to:
>
>```text
>...
>* f37ebaf Add print method to World
>* ceedb39 Add ostream operator
>| * bbff9d4 (origin/main, origin/HEAD, main) Add simulation steps
>|/
>* 81ba2ab Add daisies spreading step
>* 22dbd28 Add build folder to gitignore and remove it
>...
>```
>
>In which you can see the branching point.
>
></details>

Some commit contains messages longer than one line, keep these in mind as we will work on them in the next steps.
