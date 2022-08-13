# RayCaster game engine

This project aims to learn how the ray caster game engine works. We try to get a 
modular and optimized game engine. 

## Build the project

### Project dependencies

This Project rely on several tools to build:

| name    | minimum version | Install command (mingw & arch linux) |
|---------|-----------------|--------------------------------------|
| CMake   | 3.23            | `pacman -S cmake`                    |
| gcc     | 11              | `pacman -S gcc`                      |
| clang   | 14              | `pacman -S clang`                    |
| doxygen | 1.9.3           | `pacman -S doxygen graphviz`         |
| gcovr   | 5.1             | `pacman -S gcovr`                    |

Also, some external libraries:

| name          | minimum version | Install command (mingw & arch linux) |
|---------------|-----------------|--------------------------------------|
| freeglut      | 3.2.2           | `pacman -S freeglut`                 |
| nlohmann/json | 3.11            | `pacman -S nlohmann-json`            |
| qt            | 6               | `pacman -S qt6`                      |


### CMake command

#### Configuration phase

The base CMake command:

`cmake -S <path to source> -B <path to build directory>`

We recommend to use Ninja as build generator (install it with `pacman -S ninja`):

`cmake -S <path to source> -B <path to build directory> -G Ninja`
if the Ninja executable is not in your path just tell its location to Cmake: `-DCMAKE_MAKE_PROGRAM=<path to ninja executable>`

It is possible to set de the compiler as always with CMake: `-DCMAKE_CXX_COMPILER=<path to compiler>`

Only 2 build Type is available: `Debug` and `Release`. Optionaly, in `Debug` only, it
is possible to enable the code coverage monitoring by adding `-DRAYCAST_COVERAGE=on` to the cmake command line

#### Building phase

The build command :

`cmake -B <path to build directory> --target <desired target> -j <nb thread>`

Once configured, some targets ers now available:

| target                 | Comment                                                             |
|------------------------|---------------------------------------------------------------------|
| RayCaster              | build the game                                                      |
| RayCaster_world_editor | build the world editor utility                                      |
| documentation          | build the code documentation using doxygen                          |
| All_Tests              | run all tests in sequence (optionally generate the coverage report) |

## Theory

### Ray caster basis

Find out more technical documentation in
[Graphical basis](document/BasicRaycaster.md)

## Stuff to do

* [ ] Build system
  * [ ] Add Packaging mechanism
* [ ] refactor Maps
  * [ ] Larger variety of wall type
    * [X] first step : change color
    * [ ] second step : action on wall (doors)
  * [X] Adding load/save maps
  * [X] Add collision checks
* [ ] Refactor engine
  * [X] Make player & map creation managed by the engine
  * [ ] Adding player UI
  * [X] Map rendering
    * [X] render only the walls
    * [X] render only the seen walls
  * [X] Movement uses collision
    * [X] move collision code into Map
  * [X] Settings in external file
* [ ] Refactor input system
  * [X] Not manager by the renderer (even if same system)
  * [X] adding external key mapping (for switching keyboard)
  * [ ] key addition
    * [ ] adding a 'use' key (standard key 'e')
    * [X] adding strafe movement
    * [X] Make the draw of the map switchable (standard key 'm')
    * [X] Make the draw of rays in the map switchable (standard key 'r')
    * [ ] Go to menu (standard key 'esc')
* [ ] adding collectable objects
  * [ ] Render them
* [ ] renderer refactor
  * [ ] Allow wall texture
  * [ ] Sky dome
  * [ ] Ground texture
* [ ] Level design
  * [ ] remake Wolfenstein 3D
    * [ ] Episode 1
      * [X] Map 1
      * [ ] Map 2
  * [X] Side project: level editor
    * [X] QT based
    * [ ] Add visualization of selected cell data

