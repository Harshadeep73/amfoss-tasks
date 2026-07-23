
Level 1:

The goal was to find out a correct fruit right? I just wrote a script to handle it:

find.sh:

#!/bin/sh

DIR="/home/"REDACTED"/"REDACTED"/Terminal-Voyage-User-Edition/GrandLine/Loguetown_Reef/sector_A"

for i in $(seq 1 10)
do
    ./eat.sh "$DIR/devil_fruit_${i}.txt"
done

I just had to change the sectors. So it was just 3 iterations. But, the singular commit on devil_fruit_6 in sector_C 
blew the cover actually

I improved my micro-scripting skill through this.

Result:

Sector_C : devil_fruit_6

------------------------------------------------------------

*** CRUNCH! ***

The fruit tastes absolutely terrible...

Reality begins to fracture.

Forgotten histories rush into your mind.

You have awakened the legendary...

          Gito Gito no Mi

AWAKENING_SIGNATURE:

ONE_PIECE{GITO_GITO_NO_AWAKENING}
------------------------------------------------------------

Level 2:

I went to the whiskey_peak_investigation branch and exported the variable. Then, I ran the unlock_vault script and get the files.
Then switch to the alternate_timeline

I learnt to export variables through this.

output:

BAROQUE_DIAL{SPLIT_TIMELINE_MISDIRECTION}

Node 42.

in bounty_hunter_feed.log


Level 3:

I went to the little_garden branch for this and Once again, I just wrote down a script for this, I noticed that the correct log would be unique 
so I just used sort and uniq to isolate it.

The full script is:

#!/bin/bash

unique=$(find . -type f -exec cat {} \; | sort | uniq -u)

for file in $(find . -type f)
do
    if grep -Fxq "$unique" "$file"; then
        echo "$file"
        cat "$file"
    fi
done

Result:

./sector_beta/outpost/watchtower/storage/archive/agent_manifest.log

SECURITY LOG ACCESS // LEVEL 3 CLEARANCE REQUIRED
-------------------------------------------------
STATUS: METALLIC WAX SUIT ACTIVE

SECURITY_TAG:
QkFST1FVRV9ESUFMe1NQTElUX1RJTUVMSU5FX01JU0RJUkVDVElPTn0K

-------------------------------------------------

BAROQUE WORKS EXECUTIVE REPORT

PONEGLYPH_FRAGMENT_I = "KjY2MjF4bW0lKzYqNyBsIS0vbTAtJTcnL"

-------------------------------------------------

Level 4:

I went to the canonical branch, then the Water_7 directory and first, used gzip -dc to decompress the data into the step2_blueprints.tar file which
was mentioned if we use the file command and then kept peeling the layers off with file and the needed command based on the type until I got the secret.txt file
There I got the 2nd polyglyph frag

Result:

PONEGLYPH_FRAGMENT_II="SwnbzptDiM3JSpvFiMuJ28PJzAlJ28VIzA="


Level 5:

I went to the alternate-timeline branch, ls -a'd to find the hidden directory and ran the python file polyglyph and put it in both the frags. the prize is:

Result: 

https://github.com/rogueone-x/Laugh-Tale-Merge-War

Level 6:

I cloned the repo, went to the treasure, merged the 2 branches and manually solved the merge conflict. The password is "TheGrandLineRemembers" No spaces btw.
then I just ran the victory.sh script and entered the password. Then its done.

Result:

==============================
 Verifying Timeline Integrity 
==============================

Enter the Pirate King's Password: TheGrandLineRemembers
Timeline Integrity ............. OK
Merge Conflict ................. Resolved
Repository ..................... Restored
History ........................ Preserved

====================================================

        THE ONE PIECE HAS BEEN FOUND

====================================================

Congratulations, Captain.

The greatest treasure was never gold.

It was the ability to understand,
recover,
and preserve history.

Today you have mastered:

⚓ Linux
⚓ Git
⚓ Problem Solving

FLAG{The_Grand_Line_Remembers_Your_Commit}

====================================================

🏴 REWARD UNLOCKED

Title:
    Pirate King of Git

Badge:
    👑 Keeper of History

Your bounty has increased to

    5,600,000,000 ฿

The Thousand Sunny will always have a place for you.

Now go write your own history.



Had a lot of fun btw very fun doing this. learnt a bit of bash scripting too.
