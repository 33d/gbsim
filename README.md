gbsim is an emulator for the [Gamebuino](http://gamebuino.com/).  It's based on [simavr](https://gitorious.org/simavr/).

Usage
======

    $ gbsim <elf-file>

To get the .elf file from Arduino, go to File/Preferences, then check "Show verbose output during compiling".  Then click "Verify", and the .elf file should be shown in the output window.

Status
------

The graphics and input partially works.  Notably the [3D dungeon demo](http://gamebuino.com/wiki/index.php?title=3D_dungeon_rendering) doesn't work.  It probably uses more CPU time than it needs to.

[gdb](https://www.gnu.org/software/gdb/) support would be nice, and simavr already supports it.

What doesn't work: Everything else, including:

 - Sound
 - I&sup2;C
 - SD card
 - EEPROM
 - gdb
 - Screen brightness
 - Battery
 - Backlight

Keys
------

 - `W`, `S`, `A`, `D`: Up, down, left, right
 - `K`: A button
 - `L`: B button
 - `R`: C button
 - `ESC`: Quit

Building
======

You'll need [SDL 2](http://libsdl.org/) and [libelf](http://www.mr511.de/software/english.html).

simavr probably wants OpenGL, and Glut to build, but shouldn't be necessary.

Linux
------
    $ cmake .
    $ make

The binary will be in `build/[your-platform]`.    For Debian/Ubuntu, the dependencies are `libsdl2-dev` and `libelfg0-dev`.

Windows
------

See [README.windows.md](README.windows.md).

