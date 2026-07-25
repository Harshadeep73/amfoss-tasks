Task 3:

I had only just learnt hello world until now in rust, so this was my first proper learning of Rust and writing code with it that was actually
resposible for something working. So I am stoked to learn it as I am into systems a lot and rust was in my bucket list for a long time.


Error 1:

in /home/harsha/Projects/amfoss-tasks/Task-03/The_Grand_Line_Restoration_Initiative
/archives/reverse-mountain

found at function asset_directory_is_expected_in_config_tree().

to reproduce: run cargo test


error: asset directory not avalible in the reverse-mountain directory.

fix: either verify the directory exists and change the path, or mkdir a new
directory and then reverify

TLDR: The Directory didn't exist, just create a temporary directory and reverify and the integration test comes out fine :)



Error 2:

