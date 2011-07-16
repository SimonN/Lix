 _____________________________________________________________________________
/                                                                             \
|   Lix                                                            Read Me!   |
\_____________________________________________________________________________/



This file contains lots of general information about the game.
If you're troubleshooting a technical problem, try the file techfaq.txt.
If you're looking for info about file formats, try the file files.txt.



TABLE OF CONTENTS
-----------------

1. Overview
2. License
3. Contact
4. Command line switches
5. The level editor
6. Networked games
7. Original Lemmings levels



1. OVERVIEW
-----------

Lix is a singleplayer puzzle game, and a multiplayer action puzzle game.
It's inspired by the 1991 game Lemmings by DMA Design.

There is a level editor included in the game. You can design your own levels
and maps with it.

Play in a group of up to eight friends via LAN or the internet. Try to
lead as many lemmings as possible into your goal, even your opponents'
lemmings if they let you!

For info about the multiplayer game modes, read the proper chapter below.

If you own some of the Lemmings games, you can add their graphics and levels
to Lix. Read the corresponding chapter below for how to do it.

If you encounter any problems, read techfaq.txt.



2. LICENSE
----------

Lix is free software. You can do whatever you like with it. Alter the levels,
the graphics and even the game itself (source is available) to your pleasure,
and then give copies to everyone and their neighbor.

You cannot hold me responsible for any damage Lix does to you or your machine.
Don't save your diploma's thesis as a *.txt file and delete it using the
replay menu. ;-)

If you add game content from Lemmings games to Lix, note that the Lemmings
games are still restricted by copyright. You probably shouldn't distribute Lix
with such content already included.



3. CONTACT
----------

The Lix website is: http://lplusplus.co.cc

My email address is: eiderdaus@gmail.com

Visit the Lix IRC channel: irc.quakenet.com #lix



4. COMMAND LINE SWITCHES
------------------------

Under all operating systems, the following switches are available for the
Lix executable (./lpp.exe unter Windows, ./lpp under Linux):

-w      Run Lix in a window with a resolution of 640 x 480. If you use this
        switch, the in-game options for resolution and windowed mode are
        overridden for that session of Lix.

-n      Asks for your name when starting Lix, no matter if it is enabled or
        disabled in the game options.

-o      Runs the game without loading the sound driver. This might be useful
        under some Linux systems. To prevent loading the driver permanently,
        disable "load sound driver" in the sound options.



5. THE LEVEL EDITOR
-------------------

The level editor can be accessed via the singleplayer menu. Select any level
and hit the edit button on the right side of the screen.

You can activate the context-sensitive button help in the editor by clicking
on the button with the question mark. Hover over any editor button to see
what it is good for, or which key it has as a hotkey.

Make good use of these hotkeys. The most important hotkey is probably [C]
which copies the editor's current selection of objects. :-)

If you've created some nice levels, you can drop me a line in the Lix Forum or
via email. I've given the website and my contact info in a note above.



6. NETWORKED GAMES
------------------

Lix can be played together with friends via LAN or the internet. You may
play on the central server, or have a member of your peer group host a game.
In that case, he has to make sure that port 22934 over UDP is open.

The rules for the multiplayer game are as following. Everyone controls his
own horde of lixes, you can only assign skills to your own lixes.
Try to get as many lixes into the goal that displays your player color.
It doesn't matter whose lixes they are. You will want to lead your
opponents' lemmings in there as well for extra points...

If a player has no more lixes due to saving, losing or nuking them all,
overtime will start. This is a time limit for the other players to catch up to
his score of saved lixes. The countdown will not trigger if the
lix-less player has not yet saved a single lix. This is to prevent evil
persons from nuking their horde right at the start to spoil the game for
everyone else.



7. ORIGINAL LEMMINGS LEVELS
---------------------------

Lix can use the graphics and levels from these games:
Lemmings, Oh no more Lemmings, Holiday Lemmings, and Lemmings 2: The Tribes.
Since these games are copyright-restricted, Lix doesn't contain any of their
files. Note that if you're going to include content from those Lemmings games,
you shouldn't distribute your game directory as a whole any more.

I should write a more detailed guide sometime about which files you can use
from the copyrighted games, and where to put them in the Lix directory
structure so Lix can read their graphics. The following information in this
chapter is just about using Lemmings levels, but you must install the terrain
graphics styles if you wish to do anything with such levels. Consider asking
in IRC before you read on.

If you have built your own levels with Lemmix Level Editor, LemEdit or a
similar program, you can load these levels with Lix as well, provided you
have installed the graphics from the corresponding Lemmings games. Put your
single .LVL levels or .DAT level packs into subdirectories of the level
directory. Lix will unpack .DAT packs automatically, and determine the graphics
set based on the subdirectory structure you put the files in:

(anywhere)

    You can create any subdirectory hierarchy of levels/, put the .LVL or
    .DAT files in there, and it will work. This makes Lix read out the graphics
    set information and use it unmodified. The used set will be:

    0 = L1 Dirt
    1 = L1 Hell
    2 = L1 Pastel
    3 = L1 Pillar
    4 = L1 Crystal
    5 = ONML Brick
    6 = ONML Rock
    7 = ONML Snow
    8 = ONML Bubble
    9 = Holiday

(with "ONML"/"onml" in the path)

    Any path with "ONML" or "onml" in its name will work here. This makes Lix
    add the number 5 to the graphics set information from the file before it
    determines which graphics set to use. This is the desired behavior for
    levels taken directly from Oh no more Lemmings.
    
    However, custom-built levels in ONML-styles have the 5 already added in the
    .LVL file and thus shouldn't be added in a directory reading "ONML"/"onml".
    In any way, Lix treats the set numbers in these subdirectories as follows:

    0 = ONML Brick
    1 = ONML Rock
    2 = ONML Snow
    3 = ONML Bubble
    4 = Holiday

(with "Holiday"/"holiday" in the path)

    Any path with "oliday" in its name will work. A path like
    this currently makes Lix use the Holiday graphics set for the level.
    This is not entirely correct yet, as there are levels like
    "Vacation in Gemland" that require the ONML Rock set instead.



AND NOW
-------

Have fun playing Lix!

Simon
