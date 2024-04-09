# Excercise 1: Rebase

This excercise should familiarize you with the multiple uses of the `rebase` command, as we have seen in the lectures.

The excerise will ask you to explore the Daisyworld repository and familizarize yourself with its content, in particular the `display` branch.

Afterward, you will use `rebase` to clean up the history of the branch, before merging it back into the `main` branch.

Optionally, you can try using `stash` in conjunction with `reset` to move part of the content of a commit into a different branch.

## Explore the repository

Take some time to explore the repository and it's commits. If you want, you can try to understand what the code does but it's not necessary for the excercise.

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

## Clean the history of `display`

The first excercise is to clean up the history of the `display` branch to prepare it for the merge.

The branch contains multiple commits. You can list all and only the commits in display with a single command.

><details><summary>Reveal solution</summary>
>
>```bash
>git log --oneline main..display
>```
>
>This command will show in a pretty format, all the commit that are reachable from `display`, but are not reachable from `main`
>
></details>

If we explore the content of the repository we can notice that some commit apply changes to the same part of the code, deleting the code introduced in the previous.
If we want to clean up the history of the changes we could *squash* them all togheter.

><details><summary>Reveal the solution</summary>
>
>The commits in question are
>
>```text
>c1e670e Add correct print implementation
>e9031f1 Revert "Add print method to World"
>f37ebaf Add print method to World
>```
>
>Commit `e9031f1` is a revert of commit `f37ebaf`. Commit `c1e670e` implements the correct version of the function.
>
></details>

In addition, some commits are only quick fixes of typos. We could also decide to squash all these changes togheter.

><details><summary>Reveal solution</summary>
>
>```text
>b9675e4 Update README.md
>424f397 Ops, it is under
>4684631 Add sample output to README.md
>```
>
>Commit `4684631` adds some content to the README.md file.
>Commit `424f397` simply replace the world above with below.
>Commit `b9675e4` looks like it's written by a different author, probably from the Github user interface. It fixes a spelling mistake and adds a new line at the end of the file (notice how that is signalled in the *diff*).
>
>These three commands can all be squashed togheter.
>
></details>

Finally, a different commit make a very small change to a part of the code that is introduce in one of the first commits. We could decide to squash them togheter and make it look like the change was implemented since the beginning.

><details><summary>Reveal solution</summary>
>
>Commit `e4e908d` replace the character used in commit `ceedb39` with emojis.
>
></details>

We can do all these changes with a single run of interactive rebase.
In writing the todo list, consider which commit message to keep, and which to rewrite in the squash. It is your occasion to make the history look beautiful and intentional.

><details><summary>Reveal solution</summary>
>We can run the interactive rebase with:
>
>```bash
>git rebase -i 81ba2ab
>```
>
>We know the commit from which `display` branched from `main`. We can also use `ceedb39^`, which refers to the parent of the first commit of the `display` branch.
>
>The todo list can be set up as follows:
>
>```text
>pick ceedb39 Add ostream operator
>squash e4e908d Replace with emoji
>drop f37ebaf Add print method to World
>drop e9031f1 Revert "Add print method to World"
>reword c1e670e Add correct print implementation
>pick 1bca4f6 Add print to main function
>squash 966dc56 Add useful comment
>pick 4684631 Add sample output to README.md
>fixup 424f397 Ops, it is under
>fixup b9675e4 Update README.md
>pick 039ce60 Specify bash in code snippets
>```
>
>Commit `e4e908d` has been move forward up into the stack, and set to `squash`, to be melded in the first commit.  
>Commits `f37ebaf` and `e9031f1` have been dropped, since the second is a revert of the first.  
>Commit `c1e670e` is now the only implementation of the print function, so it can be maintained but the commit message can be slightly altered to reflect the change.  
>Commit `966dc56` can be squashed with the previous, since the change it implements is minimal, and could have easily been an *amend*.  
> Commits `424f397` and `b9675e4` are marked as fixup, since they implement very small changes and there is no need to modify the original commit message.
></details>

## Pushing changes

Now that we have changed the history we can apply the changes to our remote fork, to keep it up to date.

><details><summary>Reveal the solution</summary>
>Since these changes alter the history of the repository we have to *force push* them.
>We can see that the history are no longer compatible with each other with:
>
>```bash
>git log --all --oneline --graph
>```
>
>To do a force push we can use
>
>```bash
>git push --force-with-lease
>```
>
>Which is a more safe way to do the force push.
></details>

## (Optional) Move some changes into main

If we look at the *diff* of the last commit, we can see that it edits the README.md by specifying the language of the code blocks in it.

The changes not only apply to the sample output that was added by the commit `Add sample output to README.md` (the SHA-1 of the commit has changed now that we rebased the history!), but also to code blocks that exists in the `main` branch.

Suppose we don't plan to merge the `display` branch immediately, but we still want those edit to appear in the `main` branch.

Split the changes and apply the correct ones to the main branch. Try melding the remaining changes with the previous branch on `display`.

><details><summary>Reveal solution</summary>
>
>Let's start a rebase of the last two commits
>
>```bash
>git rebase -i HEAD~2   
>```
>
>Change the todo list to edit the last commit:
>
>```text
>pick a5c4656 Add sample output to README.md
>edit 38e4774 Specify bash in code snippets
>```
>
>This will apply the commit but stop to allow you to amend the commit before continuing the rebase.
>
>Now we can use `reset` to move back the head of the branch by one commit, keeping the changes in the working directory.
>
>```bash
>git reset --mixed HEAD~
>```
>
>We now stash the changes that should be performed on the main
>
>```bash
>git stash -p
>```
>
>Of the three hunks presented, the first two (the command blocks with the compilation commands) should be stashed, the last should be kept.
>
>We can now add the remaining change to the index, and do an *amend* to meld it with the previous commit (`Add sample output to README.md`).
>
>```bash
>git add README.md
>git commit --amend --no-edit
>```
>
>We can now conclude the rebase
>
>```bash
>git rebase --continue
>```
>
>The last thing left to do is to switch back to the main branch, apply the changes left in the stash and commit:
>
>```bash
>git switch main
>git stash pop
>git commit -a -m "Specify bash in code snippets"
>```
>
>Also in these case we will have to force push the changes to `display`, while for the new commit on `main` a regular push will suffice.
>
></details>
