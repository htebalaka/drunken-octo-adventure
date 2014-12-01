README

===============================================
Installation steps (while on the otter server):
===============================================

$ git clone https://github.com/htebalaka/drunken-octo-adventure.git
$ git branch candidate
$ cd drunken-octo-adventure/main
$ make

==============
Running steps:
==============

$ ./testboard

1. Have one player create a game, and then the other can join. The host
   needs to accept the request manually before the game starts.
2. Hitting any key might be required to make the GUI start up.
3. Move the cursor with j/k/l/i, and pickup and place pieces with u.
4. Once all pieces are placed the game will freeze until the other
   player has also placed theirs.
5. Hitting any key might be required to make the actual game start.
6. Then play the game.
   a. Pieces can move 1 space horizontally or vertically, except for
      scouts which can move any number of pieces horizontally or
      vertically, until the first obstruction.
   b. No piece can move more than 1 space and attack on the same turn,
      including scouts. Bombs and flags can't move.
   c. Spies can kill marshals, but only when attacking. They loose
      against any other piece.
      Miners are the only piece that can kill bombs.
   d. Otherwise, lower ranked pieces numerically beat higher ranked.
   e. Ties result in both the attacker and defender losing their piece.
   f. The game ends when one player loses their flag.

Known issues are documented extensively in the phase 5 project
closeout information.
