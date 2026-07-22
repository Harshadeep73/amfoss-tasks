
#Level 1:

The command I used here is git start. It just starts the level and initializes our directory with all the required stuff.

#Level 2:

The goal was to commit a file. so just add or stage the file using git add and then git commit -m "messagesmth" will be enough.

#Level 3:

One of the files should've been removed. So I used git reset and then staged 1 file using git add and then committed but git reset filename also works better.

#Level 4:

The files with o, exe and jar extensions and the whole directory of libraries have to be ignored. For this, a .gitignore has to be there and I prefer using
nano instead of echo ing in so I just nano'ed in .*extension and directoryname/ in .gitignore.

#Level 5:

The branch pointer chase-branch had to point to the same commit as escaped. Thus, we have to use git merge escaped as we are currently pointing to the chase-branch.
merge command helps us to combine the histories of both branches. No merge conflicts are present cuz chase-branch is an ancestor of escaped. Thus its a fastforward
merge.

#Level 6:

The problem here is that when we merge the branches, they modified the same piece of text in the file equation.txt. A merge conflict arises and we have to remove
the lines containing >>>>>>>>>, =========, and <<<<<<<<<< and choose to keep either or both the changes.

#Level 7:

This level is about saving our work, so we use stash, it stores our stuff in an Git object in the git/objects directory. Then we go on to fix a bug and commit it. Then we pop
back the stash.

#Level 8:

Here, we have to move the commit with the bugfix to the branch we're working on. So, we use git rebase. It helps move and apply commits. so we move and apply the bugfix
commit at the change-branch-history node.

#Level 9:

A file that shouldn't be tracked is. So, we can either use git rm to remove it from the working tree and index or we can use git rm --cached to just remove it from the
index. It does the delete staging for us.

#Level 10:

A file has to be renamed but the name's capital automatically turns into uppercase. A simple solution to this is wrapping the name in " " and using the git mv
command to move it (in our case, rename it)

#Level 11:

A past commit has to be modified here and the git commit --amend is used. This command, after the changes are staged, changes the past commit to which HEAD points to 
with the present index

#Level 12:

To change the date of the previous commit, we just have to use an extra option --date=.

#Level 13:

I used git rebase -i HEAD~3 to view the last 3 commits in the interactive rebase. Then we just change the 2nd commit to edit command instead of pick. Then, we fix
the typo and git rebase --continue is used to finish the rebase.

#Level 14:

To dig out a lost commit, we can open the logs with git reflog and find its reference. Then we git reset it to point the current branch to that specific commit its a sort 
of time traevl.

#Level 15:

Here, we have to split the commit so we first reset and point to the commit before that combined commit was made. Then we stage the first txt file and commit it then
the second file.

#level 16:

git rebase -i HEAD~1 and then squash is the command that we need to replace with pick in the second commit as it combines the commit with its previous one.

#Level 17: 

git update-index --chmod=+x script.sh this commad enables the execution bit for the script file. This makes the script.sh script executable in the index
rather than the working tree.

#Level 18:

This was a more troubling one for me. The git add -p was straight forward but the editing part was tricky. I used manual editing e option and manually selected the
lines needed for the first commit, then committed it, then again git add -p and select the rest again with e option and then commit the second one.

#Level 19:

This question also took me a little time to get a hold of. I first consecutively cherry-picked the branches and solved the merge conflict. I think the problem was
in the line selection of the merge conflict. Eventually I was able to get it. cherry-pick command is a kind of a miny rebase as u can take a commit and apply it 
on a different branch.

#Level 20:

I used git rebase --onto your-master issue-555 rebase-complex. It takes all the commits that come after issue-555 and apply it onto your-master. I learned the other
way as well from the site after solving that git rebase issue-555 --onto your-master was the fastest solution as it takes whatever commits that are not in issue-555
and applies it onto your-master

#Level 21:

To switch the two commits, the git rebase interactive needed to be used by this: git rebase -i HEAD~2 to get the last 2 commits and we just place the 2nd 
commit above the first and that's it.

#Level 22:

First, to find the commits which have introduced that word, we use log, git log -S "word" -S for searching the commit which introduced the word. Then we use
git rebase into the interactive and edit those commits. we replace the shit with flower and rebase --continue until its done.

#Level 23:

I tried to solve it by writing a bash script first and then executing it via the sh -c but it didn't work. So I just gave the bisect its bad and good commits.
The good one was the 1.0 and the bad one was the current one. The bisect uses a binary search algo that goes the mid commit and runs the checking command. It then
moves to the past or future commits based on the output of the checking command.

  

