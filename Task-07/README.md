## Task-07

# VYKE

This is my very first (functional) discord bot so I was hyped to build it. I have worked with 
data-bases before in supabase so I wasn't that troubled. I expanded it while doing the task and my
design was pretty simple,typical and generic. A simple commands and db folder.Then, the commands
folder split into 3 seperate ones: player,api and shop. The purpose is pretty self-explanatory ig
the commands regarding player is all in player.py file and so forth.

The actual idea I changed a bit. The first !bounty command, I changed it to 
!treasury. I had another purpose for !bounty in mind as a kind of a hierarchical system for pirates.
So, I did that, then pretty much everything's the same. In the end I added a couple commands:

1. `!bounty` : displays present bounty, rank and bounty required for next rank
2. `!fruit_power` : displays the power of the fruit you've consumed
3. `!eat <inventory number>` : helps the user gain the fruit type's power
4. `!sell <inventory number>` : helps the user sell items in their inventory (reduced value) to gain berries

I made the shop so that its daily and can be restocked about 9 times. Only 9 slots are avalible at
a time and once a fruit is purchased, it can be restocked only once.

Now, The interesting part, the fruit_power part. The fruits avalible in the API are Paramecia, Logia
and Zoan. SO each having some kinda power made sense in a game pov, so I just gave em powers for the 
raid command.So, each fruit has its benefits and a little con to even it out.

No fruit, Default: a 50% chance of stealing about 40% of the target's berries.40% of berries become
bounty

Paramecia: a 40% chance of stealing about 35% victim's berries and an independent 45% to have a multi-
plier and take 70% of the victim's berries. The bounty is modest at about 50% of the berries stolen

Zoan: a success rate of 70% but a reward size of 30%. But a high 85% of the berries counted into
bounty

Logia: a success rate of 40% and takes about 35% of the berries but has a rare 20% chance of stealing 
a devil fruit after a successful raid from the victim's inventory. Bounty is okay at 60%.

I had a lot of fun builing this cuz parts of my gamedev brain were working. Also, time was tight so I
finished it quick

### Fleet System

I also added a Fleet System where pirates can form and manage their own fleets. A pirate can create a 
fleet and automatically becomes its captain, while other pirates can join existing fleets. Captains 
can hand over their position to another member or disband the fleet, while normal members can leave 
whenever they want. Fleets also have their own bounty and Berries leaderboards.

I also added a simple war system between fleets. A captain can declare war on another fleet, after 
which members of the two opposing fleets can attack each other using the existing raid mechanics. 
These attacks are recorded separately from normal raids in an "Ancient Scroll of Wars", along with 
declarations of war and surrenders. This gives the fleet system a bit more of an actual gameplay 
loop rather than just being a way to group players together.

### Fleet Commands

1. `!createfleet <name>`: creates a new fleet
2. `!joinfleet <name>`: joins an existing fleet
3. `!turnback`: displays the fleet and its members
4. `!leavefleet`: leaves the current fleet
5. `!disbandfleet`: disbands the fleet (captain only)
6. `!handover @member`: transfers captaincy to another member
7. `!fleetboard <stat>`: displays the fleet leaderboard for bounty or berries

### War Commands

1. `!war <fleet>`: declares war on another fleet (captain only)
2. `!attack @member`: attacks a member of an opposing fleet during an active war
3. `!surrender <fleet>`: ends an active war by surrendering (captain only)
4. `!history`: displays the fleet's war history

##Unfinised work:

I have more ideas to improve this and make it better, like
1. A better shop system: have items having abilities revolve the shop randomly.
2. better items: items can have special abilities: one of which is protecting against a fixed number of raids or attacks.
3. More variety of fruit powers
4. A better fleet system and a more mature version of it, like having internal tests and a captain challenge (if user wins, he gets to be captain)
5. A marine role: army, legions, and their versions of fleets, and a general instead of a captain and to catch a pirate would get them their own honor or any other
   numerical value


## Resources used:

1.https://www.geeksforgeeks.org/python/python-sqlite/

2.https://www.onepieceapi.com/documentation

3.Chatgpt (learning)

I found the docs of this api [2] very approachable donno if it was due to the theme or if it was 
really that good though.
