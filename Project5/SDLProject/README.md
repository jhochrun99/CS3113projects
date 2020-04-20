# Project 5: Platformer

### Requirements
Project 5 was to create a platformer. The platformer needed to have a menu screen that displays the title. Upon pressing the `enter` key, the game would start. There also needed to be 3 levels, and the player needed to have 3 lives. If the player runs out of lives, "You lose..." pops up on the screen, and the game can be restarted by pressing `enter` to return to the menu screen. If the player gets to the end of the 3rd level successfully, "You win!" pops up on the screen, and the game can be restarted the same way. The game was required to have at least one type of moving AI that would kill the player upon collision, a looping background music, and one sound effect.

### Description
This game is called "Gem Dash". The menu screen is a view of the beginning of level 1, which has two slime enemies in it. Upon pressing `enter`, the player character spawns and the `left` and `right` arrow keys can be used to move. The `spacebar` can be used to jump, and a jump sound effect plays each time the player jumps. To get to the next level, the player needs to get to a gem located towards the end of each level. If the player loses a life before reaching the gem, they respawn at the start of the current level. If the player gets to the 3rd gem without losing all 3 lives, they win. 

Level 2 has a bat enemy that follows the player once they get within the bat's sensing range, and a slime enemy. The bottom of level 2 is all lava, and if the player falls in the lava they have a few seconds to try and jump out before losing a life. 

Level 3 has one bat enemy, and spikes across the bottom of the level. The player loses a life immediately upon touching the spikes. The player respawns one space to the left of where they start the level, so they need to move back over the 'safe' block or they'll land in spikes again. 

#### Unimplemented
* Interface that shows three gems in the top right, and three hearts in the top left. Gems would be outlines, and filled in upon completing the corresponding level. Hearts would be filled, and turn to an outline upon losing a life. 
* Level 3 was going to have spinning circular saws that would kill the player upon collision, and no bat enemy.  
* The size of the gems was going to be altered, so the player would be required to get closer to them before the level switches. 
* ENEMY_COUNT is no longer needed and was going to be removed. 

#### Music Credit
Sneaky Adventure by Kevin MacLeod
Link: https://incompetech.filmmusic.io/song/4383-sneaky-adventure
License: http://creativecommons.org/licenses/by/4.0/
