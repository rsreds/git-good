# Exercise 3: Admin tools

These exercises will focus on the use of the admin tools shown during the lecture.

In particular the first exercise will ask you to use the `git-filter-repo` tool to alter the history of the repository, while the second exercise will delve into git hooks.

## Analyze the content of the repository

Looking at the main branch of the Daisyworld repository you may have noticed that at some point in its history the `build/` folder was committed.

This is usually not intended, as version control software such as git are meant to be used with source code and no binary files.

In the history you can see there was an attempt to remove the folder, however this is not enough to delete the files from the repository itself, since they can always be restored by switching to those particular commmits.

With the command:

```bash
git rev-list --all --objects
```

we can see that indeed the repository cointains more than one object from the *build* folder.

>`git rev-list` is one of the so-called *plumbing* commands of git. It prints the list of *SHA-1* of the commits reachable from the given commit.  
The `--all` option overrides the default behaviour and list all the *refs* in the `refs/` folder as argument in the commands.  
The `--objects` option prints also all the objects referenced by the reachable commits.

As we have seen in the lecture, we can use `git-filter-repo` to give us a more advanced analysis of the repository content.

First make sure that the `git-filter-repo` script is reachable in path.

><details><summary>See details</summary>
>
>```
>git filter-repo --version
>```
>
>Shuld print the version number of the script without giving command not found errors.
>
>If `filter-repo` is not availabe as a `git` subcommand make sure to download the script from `https://github.com/newren/git-filter-repo/blob/main/git-filter-repo` and place it into your `$PATH`.  
For example:
>
>```bash
>wget https://raw.githubusercontent.com/newren/git-filter-repo/main/git-filter-repo -O ~/.local/bin/git-filter-repo
>chmod +x ~/.local/bin/git-filter-repo
>export PATH="$PATH:$HOME/.local/bin/git-filter-repo"
>```
>
>Eventually, you can install it using a package manager.  
For example:
>
>```bash
>sudo apt install git-filter-repo
>```
>
>See [the documentation](https://github.com/newren/git-filter-repo/blob/main/INSTALL.md) for additional instructions.
>
></details>

You can use `git-filter-repo` to analyze the content of the repository in more detail.

><details><summary>Reveal solution</summary>
>With the commands
>
>```bash
>git filter-repo --analyze
>cat .git/filter-repo/analysis/path-all-sizes.txt
>```
>
>we can see the list of files in the repository, reverse ordered by accumulated packed size.
>
>This shows that indeed the files in the build folder still occupies the vast majority of space in the repository.
>
></details>

## Remove large files from the repository

We could remove the `build/` folder with a `rebase` on main. However this could prove to be complex even in this simple example, as the build folder is present in multiple commits, its content changes in more than one commmit, and it gets added and removed in commits that contain other code changes. So we cannot simply drop these commits.

While a solution is possibile, in this case it's probably faster, easier and safer to use `git-filter-repo`, as we will still end up drastically changing the history.

Use `git-filter-repo` to remove the `build/` folder from the repository.

><details><summary>Reveal solution</summary>
>We first need to make sure we are working on a clean copy of our repository.
>
>Make sure you pushed all changes to your fork, then create a new clone:
>
>```bash
>git clone git@github.com:<username>/git-good-daisyworld.git clean-git-good-daisyworld
>```
>
>From there, run `git-filter-repo` specifying the path that you want to filter out of your repository. Make sure to add the option `--invert-paths`.
>
>```bash
>cd clean-git-good-daisyworld
>git filter-repo --path build/ --invert-paths
>```
>
>If we now rerun the analyze, we will see that indeed the build folder is no longer in the repository.
>
>However, if we check the logs we can see that one of the commits still reference the build folder. However from its diff we can see that it doesn't any longer affect it:
>
>```bash
>git log -p 2f0302 -1
>```
>
>```text
>commit 2f03023571ee63969f09c3913e665b5747a2ec0e
>Author: Simone Rossi Tisbeni <simone.rossitisbeni@unibo.it>
>Date:   Wed Apr 3 13:51:36 2024 +0200
>
>    Add build folder to gitignore and remove it
>
>diff --git a/.gitignore b/.gitignore
>index 722d5e7..6a8bc10 100644
>--- a/.gitignore
>+++ b/.gitignore
>@@ -1 +1,2 @@
> .vscode
>+build
>```
>
>This can simply be edited with a `rebase`, however filter-repo can also do this.
>
>Create a file containing the expression for the text to remove:
>
>```bash
>echo "and remove it==>" > ../replacement.txt
>```
>
>Then apply it to the repository using the `--replace-message` option of `filter-repo`
>
>```bash
>git filter-repo --replace-message ../replacement.txt
>```
>
>The two `filter-repo` commands can also be applied together:
>
>```bash
>git filter-repo --replace-message ../replacement.txt --path build/ --invert-paths
>```
>
></details>

Once it run, `git-filter-repo` will remove any remote ref.  
You will have to add it back before pushing your changes.

><details><summary>Reveal solution</summary>
>
>```bash
>git remote add origin git@github.com:<username>/git-good-daisyworld.git
>git push -u --force-with-lease origin main
>```
>
></details>

## (Optional) Write a pre-commit hook

Now that we removed the incriminating files we have to make sure it never happensa again. The first thing to do, is obviously to specify the `build/` folder in the `.gitignore` file, as it has already been done in a previous commit.

But this doesn't prevent to upload binary files from other sources. We could mitigate the problem with a *git hook*.

The hook should check before a commit that the committed files are non-binary and, in case some of the files are, stop the commit.

>:warning: While this prevents to commit for example the excecutable generated during compilation, it doesn't prevent to commit the numerous files that CMake writes in the build folder.  
For the puropse of this exercise it will be fine for us, but the solution is not easy. It's probably better to just educate the user :)

When you initialize a repository, git will add some sample hook in the `.git/hook` folder.

Find which hook to modify to perform the check.

><details><summary>Reveal solution</summary>
>
>The list of sample hooks is pretty big:
>
>```bash
>ls .git/hooks/
>applypatch-msg.sample   post-update.sample  pre-merge-commit.sample pre-receive.sample  update.sample
>commit-msg.sample   pre-applypatch.sample   pre-push.sample prepare-commit-msg.sample
>fsmonitor-watchman.sample   pre-commit.sample   pre-rebase.sample   push-to-checkout.sample
>```
>
>The one that interests us is the `pre-commit` hook.
>
></details>

The sample commit presents a way to check that new files committed are not named with non-ascii character.
Try to understand what the different lines of the script do:

- L1
- L10:16
- L19
- L31:23

><details><summary>Reveal solution</summary>
>
>- L1  
>Contains the *[shabang](https://en.wikipedia.org/wiki/Shebang_(Unix))* necessary to set the file as executable using the shell.
>- L10:16  
>The `--verify` option for `rev-parse` checks that exactly one reference to an object exists. In this case is used to check if *HEAD* refers to something.  
>Indeed in an empty repository we saw in the lectures that *HEAD* points to a *head* that still is not accessible.  
>It set the `against` variable to an empty tree object in that case.
>- L19  
>git allows for custom configuration options, that can be easily created and referred in any moment. Non existing options will return exit code 1.
>- L31:32  
>The `diff` command list the name of the files (`--name-only`) that have been added(`--diff-filter=A`) to the staging area (`--cached`).  
>The `--diff-filter` accepts [various options](https://git-scm.com/docs/git-diff#Documentation/git-diff.txt---diff-filterACDMRTUXB82308203).  
>`-z` is used to qoute pathnames with unusual characters; and `$against` is the commit against which the diff is being made.  
>`LC_ALL=C tr -d '[ -~]\0'` removes all ASCII printable characters and null characters from the input and `wc -c` counts the remaining ones.  
>If the command returns non-zero, `test` will exit with an error code.
>
></details>

Let's alter the hook to check for binary files.
One way to do it is to use:

```bash
file --mime-encodig ${filename} | grep binary
```

The `file` command determines the filetype; with `--mime-encoding` option it output the *mime* encoding type string, for example `us-ascii` or `binary`.

><details><summary>Reveal solution</summary>
>A possible function to check if the file are not binary is:
>
>```bash
>checkForBinaries() {
>    binaryFilesCount=0
>    for file in $(git diff --cached --name-only --diff-filter=d) ; do 
>        binaryFlag=
>        if [ ! -z "$(file --mime-encoding ${file} | grep binary)" ] ; then
>            printf "Commit aborted. Found binary file.\n"
>            exit 1
>        fi
>    done
>}
>```
>
>We can now add this function and a call to it to the existing `pre-commit.sample`.
>
></details>

Finally make sure that the hook will run and test it by trying to commit for example the daisyworld executable.

><details><summary>Reveal solution</summary>
>
>The sample hook must first be renamed to `pre-commit`, then set as executable.
>
>```bash
>mv .git/hooks/pre-commit.sample .git/hooks/pre-commit
>chmod +x .git/hooks/pre-commit
>```
>
>We can now test it:
>
>```bash
>git add -f build/daisyworld
>git commit
>```
>
>We will have to use the `-f` option to add to bypass the `.gitignore` when adding files from the build folder.  
>
>If everything worked correctly we will have an output like this and the commit will not go through:
>
>```text
>Commit aborted. Found binary file.
>```
>
></details>

The solutions folder presents a possible completed pre-commit hook. 
