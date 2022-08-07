# RayCaster game engine

This project aims to learn how the ray caster game engine works. We try to get a 
modular and optimized game engine. 

## Graphical engine basis

Find out more technical documentation in
[Graphical basis](document/BasicRaycaster.md)

## Stuff to do

* [ ] refactor Maps
  * [ ] Larger variety of wall type
    * [ ] first step : change color
    * [ ] second step : action on wall (doors)
  * [ ] Adding load/save maps
* [ ] Refactor engine
  * [ ] Make player & map creation managed by the engine
  * [ ] Adding player UI
  * [ ] Map rendering
    * [ ] render only the walls
    * [ ] render only the seen walls
* [ ] Refactor input system
  * [ ] Not manager by the renderer (even if same system)
  * [ ] adding external key mapping (for switching keyboard)
  * [ ] key addition
    * [ ] adding a 'use' key (standard key 'e')
    * [X] Make the draw of the map switchable (standard key 'm')
    * [X] Make the draw of rays in the map switchable (standard key 'r')
    * [ ] Go to menu (standard key 'esc')
* [ ] adding collectable objects
  * [ ] Render them
* [ ] renderer refactor
  * [ ] Allow wall texture
  * [ ] Sky dome
  * [ ] Ground texture

