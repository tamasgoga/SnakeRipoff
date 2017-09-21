[Snake](https://en.wikipedia.org/wiki/Snake_(video_game) "Wikipedia article") clone, by _Goga Tamas_.

## BUGS ##
1. **Bug at button removal** (working now, but in a hacky way). Check commented out ~Button in `game/entities.hpp, line 47`.

## ROADMAP ##
- **0.8:**
  - Credit myself (bottom right on menu screen);
  - Actual random place for the snake to start from;
  - Vicinity food generation;
  - Support multiple (hopefully any) resolutions;
  - ~~TTF: changeText() -> load in same place~~ -- _it already works like that_;
  - Effects when:
    - ~~The snake eats food;~~
    - Game speed increases;
  - Gameplay: speed up over time (this completes classic mode).
- **0.9:**
  - Endless mode with events and powerups;
  - Sound (Old-school bleeps. Melody based on the snake's actions);
  - Store highscores;
  - Add notifications/warnings (like "Are you sure you want to quit?").
- **??:**
  - AI for the snake;
  - Proper ending.

- **1.0:**
  - Test, fix bugs, remove unused code...

## FUTURE IDEAS ##
- Gameplay:
  - Wall tiles;
  - Map editor;
  - Vicinity food generation.
- Options:
  - Full-screen.
- **No more _Core_ reworks.**

## LICENSE ##

> **All right reserved**, until I choose a license (something open-source, like MIT, most probably).

## CHANGELOG ##
- **0.7 "Classy Folded Code":**
  - _Project resumed on July 26, 2017;_
  - Reorganized the source files.
- **0.7 "Classy Code":**
  - _Project resumed at the beginning of September, 2016;_
  - More classes, I presume.
- **0.6 "Polish Party":**
  - Old code reorganized;
  - Polishing touches added;
  - Menus (hard coded), buttons, sliders were added.
- **0.5:**
  - _Project started in the 2nd quarter of 2016;_
  - First version, because no project should start from zero (plus, a most of the Core library was done);
  - A functioning, rough prototype of the game.
