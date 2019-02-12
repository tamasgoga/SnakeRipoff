[Snake](https://en.wikipedia.org/wiki/Snake_(video_game) "Wikipedia article") clone, by _Goga Tamás_.

![gitimage.png](https://raw.githubusercontent.com/tamasgoga/SnakeRipoff/master/res/gitimage.png)

## CONTROLS ##
Use the ```arrow keys``` to move the snake. ```F1``` toggles _help mode_ (grid tiles become more clearly defined).

## DEPENDENCIES ##
```SDL2```, ```SDL2-ttf``` -- [Setting up SDL2](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php); [Setting up SDL extensions](http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php)

## LICENSE ##
> Distributed under the MIT license.

## CHANGELOG ##
- **1.0.4 "Release":**
  - Tried out some ideas, decided they would take too long;
  - Brought the code back to a semi-stable state;
  - Probably the last version to the game _(12th Feb 2019, 11:32)_.
- **1.0.3 "Release":**
  - A bit better random generation;
  - Improved color-palette.
- **1.0.2 "Release":**
  - Started re- factoring/thinking the game code as it is very unorganized (was not the main focus of dev up til now).
- **1.0.1 "Release":**
  - Reverted the "better" food generation as it was buggy to the old, safe-but-potentially-slow version.
- **1.0.0 "Release":** _(15th Jan 2019, 14:00)_
  - Better random food generation;
  - At long last, we're here. ^_^
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

## FUTURE IDEAS (that will never happen) ##

These things will never happen, but I'd like to keep them for my own future amusement.

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
