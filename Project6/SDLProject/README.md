# Project 6: Final

### Requirements
Project 6 was up to us to choose, with the restriction that we could not make another 2D platformer. We also had to use new assets (art, music, sound) and could not use any of the textures from the lectures. 
The game needed to have a menu screen displaying the title, and the game would start upon pressing `enter`. There needed to be ~two minutes of gameplay, some type of moving AI, and at least one looping background music and one sound effect. There also needed to be a way for the player to lose (touching an AI, falling off a ledge, running out of time) and a way for them to win (killing all the AI, collecting all the coins, whatever you want).
<br>**_Collaborated with Angie Beck._**

### Goals / Description
* Title: Dig Deeper
* Player can only move left, right, and down, using the `left arrow key`, `right arrow key`, or `down arrow key`. If there's a block in the way, that block will be destroyed.
* Gravity exists - if the player destroys the block beneath them, they fall
* Different blocks interact differenty:
   - Dirt is the standard block, and can be destroyed 
   - Metal can't be destroyed / removed
   - Sand falls if nothing is under it, and will kill the player if they go beneath it as it falls
   - Glass breaks if the player falls or stands on top of it too long
* There are also different terrain hazards: 
   - Lava kills the player 
   - Spikes kill the player
* Possible enemies:
   - Slime: moves back and forth, can be killed if the player lands on top of it. Touching it any other way will kill the player. Slime will stay on the level it starts on, so player can dig deeper to escape it. 
* Sound effects:
   - Destroying a block
* Player wins if they reach the castle at the bottom
* If player dies 3 times, they get game over and must start from the beginning
* Along the way will be tower 'check points'. If the player touches these towers, they'll be sent back to the it upon death. Without a check point, they'll be sent back to the beginning

#### Music Credit
* Call to Adventure by Kevin MacLeod <br>
Link: https://incompetech.filmmusic.io/song/3470-call-to-adventure <br>
License: http://creativecommons.org/licenses/by/4.0/
* "Digging, Ice, Hammer, C.wav" by InspectorJ (www.jshaw.co.uk) of Freesound.org
