# Demo: Zombie survival

- Deadline: end of September

## Gameplay

- Player controls a wizard that can move in 4 directions
- Player can move into a one way portal teleports him to another place on the map
- Zombies spawn from specific locations and always follow the wizard
- Wizard can shoot a fireball in the direction he's facing
- If fireball connects with a zombie, the latter dies and fireball disappears
- The whole level is seen in the window
- Game is ended if a zombie touches the wizard
- Display score in a resizable way
- There are walls randomly placed inside the level that block all movable units
- Portals close and reopen in a random period
- Goal: survive and get as much score as possible (score is given for each killed zombie)

## Engine features required

- Display background below other sprites
- Wizard, zombies fireball and teleport have to be animated
- Game level is fit in screen
    - Resizable window
    - Scale resolution without altering aspect ratio
    - Resize sprites accordingly, but do not distort them
- Have a command queue that can process events in the game
    - Zombie touching the wizard, fireball connecting with a zombie
    - Process keyboard events
- Text rendering with resolution
- Fixed, stable FPS
- Provide timer mechanism inside the engine
- Collision detection
