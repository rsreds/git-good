# Exercise 3: Admin tools

These exercises will focus on the use of the admin tools shown during the lecture.

In particular the first exercise will ask you to use the `filter-repo` tool to alter the history of the repository, while the second exercise will delve into git hooks.

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
>cat .git/filter-repo/analyze/path-all-sizes.txt
>```
>
>we can see the list of files in the repository, reverse ordered by accumulated packed size.
>
>This shows that indeed the files in the build folder still occupies the vast majority of space in the repository.
>
></details>

## Remove large files from the repository

We could remove the `build/` folder with a `rebase` on main. However this could prove to be complex even in this simple example, as the build folder is present in multiple commits, it's content changes in more than one commmit, and it gets added and removed in commits that contain other code changes. So we cannot simply drop these commits.

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
>However, if we check the logs we can see that one of the commits still reference the build folder. However from it's diff we can see that it doesn't any longer affect it:
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
>```bash
>git remote add origin git@github.com:<username>/git-good-daisyworld.git
>git push -u --force-with-lease origin main
>```
></details>

## (Optional) Write a pre-commit hook

Now that we removed the incriminating files we have to make sure it never happensa again. The first thing to do, is obviously to specify the `build/` folder in the `.gitignore` file, as it has already been done in a previous commit.

But this doesn't prevent to upload binary files from other sources. We could mitigate the problem with a *git hook*.

The hook should check before a commit that the committed files are non-binary and, in case some of the files are, stop the commit.

>:warning: While this prevents to commit for example the excecutable generated during compilation, it doesn't prevent to commit the numerous files that CMake writes in the build folder.  
For the puropse of this excercise it will be fine for us, but the solution is not easy. It's probably better to just educate the user :)


