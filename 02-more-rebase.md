# Exercise 2: More rebase

This excercise will focus on the rebase command to merge changes back to `main`.

Optionally, you can test again your `rebase` knowledge by using it to split a commit in two.

## Rebase `display` on `main`

If we look at the repository history we can see that the `display` branch is not a simple continuation of the `main` branch and that at some point the two branches diverged.

We can use git log to see the history graph.

><details><summary>Reveal solution</summary>
>
>```bash
>git log --oneline --graph --all</code>
>```
>
>```text
>* 63a1d41 (HEAD -> main) Specify bash in code snippets
>* bbff9d4 Add simulation steps
>| * ba80000 (display) Add sample output to README.md
>| * e69a7c7 Add print to main function
>| * 6e42d59 Add print implementation
>| * 967c2bb Add ostream operator
>|/
>* 81ba2ab Add daisies spreading step
>* 22dbd28 Add build folder to gitignore and remove it
>* c7f30f6 Implement temperatures diffusion
>* 37eedd6 Add compilation instructions to README.md
>* 54fc8c9 Add Cmake configuration
>* 2d2c7d3 Implement local heating due to Albedo
>* 3be66b1 Initial implementation of World and Daisy
>* fc27fb5 Add initial daisyworld description
>* bc4105a Initial implementation of cpp project
>* 491484f Initial commit
>```
>
></details>

Since a merge in these condition would not be clean, the best way forward is first to incorporate the changes from `main` into our `display` branch.

We can do that simply with rebase.

><details><summary>Reveal solution</summary>
>First we make sure we are in the `display` branch, then we simply call rebase to apply the commits on top of the *head* of `main`
>
>```bash
>git switch display
>git rebase main
>```
>
>We will incur on a merge conflict. Let's deal with this by fixing the conflicting part of the code, than continue with the rebase.
>
> >! A conflict is marked as solved when all the `<<<<<<<`, `=======`, and `>>>>>>>` markers are removed.
>
>A possible implementation of the `main.cpp` that fix the conflict is available in [solutions](/solutions/main.cpp)
>
>```bash
>...after editing main.cpp
>git add main.cpp
>git rebase --continue
>```
>
>Launching `git rebase --continue` right after the `add` will make sure that there is a (interrupted) rebase in progress before calling git commit.
>
></details>

With the rebase successfull, our history now will look much more clean than before, and all the commits in the `display` branch will follow directly the head of the `main` branch.

## Merge `display` on `main`

Now that the display branch is up to date with main, we can merge the two togheter.

><details><summary>Reveal solution</summary>
>
>First we move to the `main` branch.
>
>```bash
>git switch main
>```
>
>To merge the changes we will have to use the merge command.
>
>```bash
>git merge display
>```
>
>Since the `display` branch has a linear history with the main branch, git will use the fastforward, and simply move the *head ref* of `main` to point at the same commit of `display`.
>
></details>

We can now push the changes, and eventually delete the display branch.

><details><summary>Reveal solution</summary>
>
>```bash
>git push
>git branch -d display
>```
>
>the `-d` option works only if the branch has already been merged, otherwise it will not get deleted.
></details>

## (Optional) Split a previous commit

If we look at the `main` history, we can see that one commit contains multiple changes. It would be best to split this commit, to make the history more clear.

Using the tool at your disposal try to split the incriminating commit. You should be able to indentify it quickly, from its commit message.

><details><summary>Reveal solution</summary>
>Looking at the logs we can find the commit:
>
>```bash
>git log -1 3be66b1
>```
>
>```text
>commit 3be66b1f51e85c49f9251231de4f48f90fd4dd80
>Author: Simone Rossi Tisbeni <simone.rossitisbeni@unibo.it>
>Date:   Wed Apr 3 11:51:26 2024 +0200
>
>    Initial implementation of World and Daisy
>
>    This commit adds both the World and Daisy classes to the project.
>    It is better to split the implementation of these two classes
>    into separate commits.
>```
>
>We now start a rebase that reach that commit by referencing its parent:
>
>```bash
>git rebase -i 3be66b1^
>```
>
>set the todo list on that commit (it will be the first item) to `edit`.
>
>This will put us out to the command line after applying the commit.
>
>Now we can use `reset` to move back the head of the branch by one commit, keeping the changes in the working directory.
>
>```bash
>git reset --mixed HEAD~
>```
>
>We can explore the changes using
>
>```bash
>git diff
>```
>
>We now stage and commit the changes that we want to add to the first commit:
>
>```bash
>git add daisy.hpp
>git commit -m "Add initial implementation of Daisy"
>```
>
>and finally the changes belonging to the second commit
>
>```bash
>git add world.cpp world.hpp main.cpp
>git commit -m "Add initial implementation of World
>```
>
>We are now ready to finish the rebase:
>
>```bash
>git reabse --continue
>```
>
>>! If we have run the daisyworld simulation before, we could find in our repository a `build/` folder that we should not add to the repository.  
>>However you may notice that the build/ directory was not yet added to the `.gitignore` file.  
>>For now, simply remove it to allow the rebase to continue without errors.
>
>Now that we changed the history of our main branch we are again forced to do a push with the --force-with-lease option.
></details>
