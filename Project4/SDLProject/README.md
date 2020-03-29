<h1>Project 4: Rise of the AI</h1>

Project 4 was to make a game including 3 enemies with different behavior (walking, patrolling, jumping, shooting, etc). All the
enemies must be defeatable - the Player needs to be able to clear the screen. If the Player clears all enemies, they win.
If an enemy touches the Player, they lose. 

My enemy types are:
* Slime : moves back and forth once the game is started, Player jumps on it to defeat it.
* Bat : hangs idly until the Player gets within its sensing range, then tracks the Player. Player jumps on it to defeat it. 
* Fire : sits in the center of the screen, unmoving. Player dies if they touch any part of the fire; it is defeated by hittig
a button and turning it "off".

In my game, the Player is controlled with the `left` and `right` arrow keys, and the `spacebar`.

Start the game by hitting the `left shift` key. The game can be reset by pressing the `left shift` key, 
and started again by pressing the `left shift` key a second time.

Note that the Fire enemy is also supposed to shoot fireballs. The fireballs are shot in a random direction with varying speeds. 
They disappear when they hit a platform, or end the game if they hit the Player. The Player does not need to 'defeat' the 
fireballs, as they are not enemies on their own. 
The fireballs are still being worked on: currently, they occasionally linger after hitting a platform, and on the first play of
the game, they all shoot at once. Once the game is restarted, they shoot one at a time as intended. 
