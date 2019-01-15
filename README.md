[Snake](https://en.wikipedia.org/wiki/Snake_(video_game) "Wikipedia article") clone, by _Goga Tamás_.

## CONTROLS ##
Use the ```arrow keys``` to move the snake. ```F1``` toggles _help mode_ (grid is clearly defined), for now.

## DEPENDENCIES ##
```SDL2```, ```SDL2-ttf``` -- [Setting up SDL2](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php); [Setting up SDL extensions](http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php)

## ROADMAP ##

These are the updates that are going to happen sooner or later.

- **1.0:**
  - Better random food generation;
  - Final testing, polish & potential bugfixes.

## FUTURE IDEAS ##

These things might never happen. They're here for future reference.

- Gameplay:
  - Endless mode with events and powerups;
  - "Vicinity food generation";
  - Wall tiles;
  - Map editor.
- Options:
  - Support multiple (or any) resolutions;
  - Full-screen.
- Misc:
  - Add notifications/warnings (like "Are you sure you want to quit?");
  - Sound (Old-school bleeps. Melody based on the snake's actions);
  - Actual random place for the snake to start from;
  - AI for the snake.
- **No more _Core_ reworks.**

## LICENSE ##
> Distributed under the MIT license.

## CHANGELOG ##
- **0.8.2 "Beta":**
  - Completed scoreboard page.
- **0.8.1 "Beta":**
  - Added help mode & fiddled around with the code for old times sake.
- **0.8.0 "Beta":**
  - Finished an OK looking scoreboard;
  - _Project in BETA on August 31, 2018._
- **0.7.18 "Almost There":**
  - Primitive scoreboard UI;
  - Gave up on working with date in C++, too much hassle for a small project like this;
- **0.7.17 "Almost There":**
  - Highscore save feature now works, needs to be put in UI, though!
- **0.7.16 "Almost There":**
  - Added a very bare-bones high-score save feature (not done yet).
- **0.7.15 "Classy Cleanup":**
  - Effect when game speed increases;
  - UI revision.
- **0.7.14 "Classy Cleanup":**
  - _Project resumed on August 29, 2018;_
  - Remove  ```SDL2-image```  dependency;
  - Revised the road-map to reflect something I'll actually accomplish.
- **0.7.13 "Classy Cleanup":**
  - Reorganized code & split files in ```game/```.
- **0.7.12 "Classy Cleanup":**
  - Removed unnecessary resources.
- **0.7.11 "Classy MIT Code":**
  - License changed to MIT.
- **0.7.x "Classy Folded Code":**
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
