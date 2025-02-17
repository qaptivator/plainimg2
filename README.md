# plainIMG2

_///// as simple as it gets for an image viewer /////_

# usage

- install the executable from the latest release (as of now, just clone the repository)
- run it by using the context menu button in an image (may require you to click "show more")
  - or by dragging a picture over the executable
  - or by running the executable and opening the picture with `Right Click > Open Image...` (or just letter `O`)
  - or by running it through the terminal with first argument being the file path

the main menu is opened with `Right Click`.  
you can also close the window with `Right Click > Quit` or just `Q`, or just `ESCAPE`.
to make the window be the size of the image, press `R`.
to make the window always be on top (dont go behind other windows when unfocused), press `T`. you can change it back any time. _this is enabled by default_

# introduction

sometimes, i just want to open an image, and pin it somewhere on my display.  
i can open images with other progrsms like built-in media app or some external one,  
but there is one thing i always encounter: **application window ui**

a lot of the times, the ui gets in the way of me just SEEING the image.  
so with this tool, i fixed just that!  
using this tool, you can view the image easily, because it covers the entire window!
so now, without any obtrusive ui, you can view your images.  
you can also make the window be always an top, so its actually usable (inspired by AlwayOnTop module in PowerToys).

at first, i made this in python, but pyinstaller executables were always flagged by antiviruses, so i decided to remake the project in C with SDL.
this not only makes the project not false flagged by antiviruses, but also more lightweight!

# build

- SDL3 is included inside this repository, so just clone this repository
- you need vs c/c++ redistributable and gcc installed
- run `./build.sh`, and it will build everything for you with gcc. run with `./plainIMG.exe` (or `./build.sh && ./plainIMG.exe` for short)

# todo list

- [ ] save menu configurations to some text file, so that it will save between launches
- [ ] add a reminder when there is a new version of this app
- [ ] finish up lite mode
- [ ] add icon to the executable with windres
