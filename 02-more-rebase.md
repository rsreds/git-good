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
>First we make sure we are in the correct branch, then we simply call rebase to apply the commits on top of the *head* of `main`
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
