# Project 6: Final

### Requirements
Project 6 was up to us to choose, with the restriction that we could not make another 2D platformer. We also had to use new assets (art, music, sound) and could not use any of the textures from the lectures. 
The game needed to have a menu screen displaying the title, and the game would start upon pressing `enter`. There needed to be ~two minutes of gameplay, some type of moving AI, and at least one looping background music and one sound effect. There also needed to be a way for the player to lose (touching an AI, falling off a ledge, running out of time) and a way for them to win (killing all the AI, collecting all the coins, whatever you want).
<br>**_Collaborated with Angie Beck._**

### Goals / Description
* Title: Dig Deeper
* Menu screen: flashes through examples of how different blocks work
* Player can only move left, right, and down. Pressing the `left arrow key`, `right arrow key`, or `down arrow key` moves the player one block to the left/right/down. If there's a block in the way, that block will be destroyed and the player move into the destroyed block's space. 
* Gravity exists - if the player destroys the block beneath them, they fall
* Different blocks interact differenty:
   - Dirt is the standard block 
   - Metal can't be destroyed / removed
   - Sand falls if nothing is under it, and will kill the player if they go beneath it as it falls
   - Glass breaks if the player falls on top of it
* There are also different terrain hazards: 
   - Lava will kill the player 
   - Spikes kill the player if they land on top of them
   - If the player falls more than 2 blocks at a time, they die
* Possible enemies:
   - Slime: can be killed if the player lands on top of it. Touching it any other way will kill the player. Slime will stay on the level it starts on, so player can dig deeper to escape it
   - ?
* Sound effects:
   - Destroying a block
   - Dying
* Player wins if they reach the bottom
* If player dies 3 times, they get game over and must start from the beginning
* Along the way will be 'check points'. If the player gets these check points, they'll be sent back to the check point upon death. Without the check point, they'll be sent back to the beginning

Possible terrain pack: https://www.kenney.nl/assets/map-pack

#### Unimplemented
* TBD

#### Music Credit
* Call to Adventure by Kevin MacLeod <br>
Link: https://incompetech.filmmusic.io/song/3470-call-to-adventure <br>
License: http://creativecommons.org/licenses/by/4.0/
* "Digging, Ice, Hammer, C.wav" by InspectorJ (www.jshaw.co.uk) of Freesound.org
