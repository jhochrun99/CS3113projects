<h1>Project 4: Rise of the AI</h1>

Project 4 was to make a game including 3 enemies with different behavior (walking, patrolling, jumping, shooting, etc). All the
enemies must be defeatable - the player needs to be able to clear the screen. If the player clears all enemies, they win.
If an enemy touches the player, they lose. 

My enemy types are:
* Slime : moves back and forth once the game is started, player jumps on it to defeat it.
* Bat : hangs idly until the player gets within its sensing range, then tracks the player. Player jumps on it to defeat it. 
* Fire : sits in the center of the screen, unmoving. Player dies if they touch any part of the fire; it is defeated by hittig
a button and turning it "off".

In my version, the player is controlled with the `left` and `right` arrow keys, and the `spacebar`.

Start the game by hitting the `left shift` key. The game can be reset by pressing the `left shift` key, 
and started again by pressing the `left shift` key a second time.
