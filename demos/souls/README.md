# Demo: Souls

- Deadline: end of 2019

## Gameplay

- Level full of doors/passages, which require a certain amount of souls
- Fog of war beyond a certain radious around the player
- Can see only a portion of the level, camera follows the player
- Demons shoot projectiles at player, even outside the visible area
- Hack and slash type of game
    - Player is equipped with a sword that has an attack animation
        - Player can pick up a different melee weapon later
        - Use sword to kill demons, when you are near them
        - You can collect their souls in a certain amount
    - Player moves to the point which is clicked, unless there is an enemy
        - Player will attack the enemy instead
- Full screen and automatically detect resolution
    - Ratio of images has to be preserved
    - View depends on the resolution
- Isometric world

## Engine features required

- Automatic conversion between isometric and screen coordinates
- Layered (complex) sprites: weapon + player
- Finding game objects with click coordinates
- Camera control
- Fog of war
    - Do not draw objects that are not visible
- Automatic resolution setting
    - View ratio + streching without ratio loss

## Additional features

- Ability to install the engine to be able to use it in other projects
