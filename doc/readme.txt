 _____________________________________________________________________________
/                                                                             \
|   Lix                                                            Read Me!   |
\_____________________________________________________________________________/



This file contains lots of general information about the game. There are other
files about special topics in this directory.

If you're looking for info about compiling the game, see linux.txt.



TABLE OF CONTENTS
-----------------

1. Overview
2. Copying Information
3. Contact
4. Command line switches
5. The level editor
6. Networked games
7. Original Lemmings terrain and levels



1. OVERVIEW
-----------

Lix is a singleplayer puzzle game, and a multiplayer action puzzle game.
It's inspired by the 1991 game Lemmings by DMA Design.

There is a level editor included in the game. You can design your own levels
and maps with it.

Play in a group of up to eight friends via LAN or the internet. Try to
lead as many lixes as possible into your goal, even your opponents' lixes
-- should they let you.

For info about the multiplayer game modes, read the proper chapter below.

If you own some of the Lemmings games, you can add their graphics and levels
to Lix. Read the corresponding chapter below for how to do it.

If you encounter any problems, read techfaq.txt in this directory.



2. COPYING INFORMATION
----------------------

Lix is placed in the public domain via the CC0 public domain dedication.
See the file copying.txt in this directory for details.

You can do whatever you like with Lix. Alter the levels, graphics, sound
effects and even the game itself (source is available) to your pleasure,
and then give copies to everyone and their neighbor.

You cannot hold me responsible for any damage Lix does to you or your machine.
Don't save your master's thesis as a *.txt file and delete it using the
replay menu. ;-)

If you add game content from Lemmings games to Lix, note that the Lemmings
games are still restricted by copyright. You probably shouldn't distribute Lix
with such content included.



3. CONTACT
----------

The Lix website: http://asdfasdf.ethz.ch/~simon

My email address: eiderdaus@gmail.com

Visit the Lix IRC channel: irc.quakenet.com #lix

If you have an account on the Lemmings Forums, http://www.lemmingsforums.com,
you can send me a personal message there. My username is Simon.



4. COMMAND LINE SWITCHES
------------------------

Under all operating systems, the following switches are available for the
Lix executable (./lix.exe unter Windows, ./bin/lix under Linux):

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

The editor has a status bar. To learn the functions and hotkeys of the various
buttons, hover over any of them with the mouse.

You can change the default hotkey bindings in the game's options. Make good
use of the keys: The handiest hotkey is the copy key, and popping up the
terrain browser is also common.

If you've created some nice levels, you can drop me a line in the Lix Forum or
via email. I've given the website and my contact info in a note above.



6. NETWORKED GAMES
------------------

Lix can be played together with friends via LAN or the internet. You may
play on the central server, or have a member of your peer group host a game.
In that case, he has to make sure that port 22934 over UDP is open.

The rules for the multiplayer game are as following. Everyone controls his
own tribe (color) of lixes, you can only assign skills to your own lixes.
In a team game (when a color is picked by several people), the teammates share
the skillset.

Try to get as many lixes into the goal that displays your player color.
It doesn't matter whose lixes they are. You will want to lead your
opponents' lixes in there as well for extra points...

If a player has no more lixes due to saving, losing or nuking them all,
overtime will start. This is a time limit for the other players to catch up to
his score of saved lixes. The countdown will not trigger if the
lix-less player has not yet saved a single lix. This is to prevent evil
persons from nuking their tribe right at the start to spoil free-for-all
games for everyone else.



7. ORIGINAL LEMMINGS TERRAIN AND LEVELS
---------------------------------------

Lix can use the graphics and/or levels from these games:
Lemmings, Oh no more Lemmings, Holiday Lemmings, and Lemmings 2: The Tribes,
and it can read levels in Lemmini's .ini format. Since the Lemmings games are
copyright-restricted, Lix doesn't contain any of their files.

If you have some of the Lemmings games, you can use their terrain in Lix.
Read the file oldterr.txt in this directory for detailed instructions.



AND NOW
-------

Have fun playing Lix!

Simon
