# [plainIMG2](https://github.com/qaptivator/plainimg2)

_///// as simple as it gets for an image viewer /////_

# why would i use this?

1. look at the comparison between plainIMG and Photos (opens images almost twice as fast!):

![plainIMG_open_comparison](examples/plainIMG_open_comparison.gif)

2. the image window is pinned on top of every other window by default (no need to install PowerToys or other software!):
3. lightweight (the executable is under 3MB!)

# usage

- install the installer from the latest release.
- run it and it will install the app, and also add plainIMG to start menu. if you click the "Set plainIMG as the default image viewer" option, jpg, jpeg, png and bmp file extensions will be associated with plainIMG.
- now that it is installed, you can open an image, select plainIMG and click "Always".
- alternatively, you can go to settings, default apps, and set common image formats to plainIMG.
- when you are done, you can use plainIMG by clicking on the image (or using "Choose an App on your PC")
  - or by running the executable and opening the picture with `Right Click > Open Image...` (or just letter `O`)
  - or by running it through the terminal with first argument being the file path
  - or by dragging a picture over the executable

the main menu is opened with `Right Click`.
you can close the window with `Right Click > Quit` or just `Q`, or just `ESCAPE`.
to make the window be the size of the image, press `R`.
to make the window be over other windows and also dont go behind other windows when unfocused, press `T`. _this is enabled by default._  
to see the rest of the features, look down below.  
to support the developer, you can appreciate the project by giving it a star in the GitHub repository. thanks!

# all of the features

- open an image using in-app file selector with `O`,
  - or give it the image path as first argument,
  - or as a hidden windows feature, drag an image over the executable and it will consider it as the first cli argument.
- close an image with `C`, which puts you back into the starting menu.
- open actions menu with `Right Click`, which shows all of the actions which you can do and their corresponding hotkeys.
- make the windows stay on top of other windows using `T`. this basically pins the window, and it wont disappear when you unfocus out of the window. enabled by default.
- use a black background instead of white one with `B`. useful for png files. disabled by default.
- keep the aspect ratio of the image with `A`. if you disable this, it will stretch the picture across the entire window. enabled by default.
- resize the window to be the image's size with `R`. note: only works if keep aspect ratio is enabled.
- when working with small resolution images, antialiasing usually ruins the image, so disable it with `L`. antialiasing is enabled by default.
- the window is also draggable in client area (other than the title bar), but be aware that native windows functions like window snapping wont work here. they only work with the title bar provided by windows.
- quit the app by closing the window, pressing `Q` or `ESCAPE`.
- the link to this github repository is located inside the `About` menu.
- WARNING: as of now, this project is only available on Windows. if this project gets enough attention, i will make a version for macOS and possibly even Linux.
- note: when you click the "Set plainIMG as the default image viewer" option in the installer, it automatically sets plainIMG as default for: .jpg, .jpeg, .png and .bmp; if you want to extend this list, update file extension defaults yourself in windows settings.
- TIFF, AVIF and WEBP are not supported.

# screenshots

![screenshot1](examples/screenshot1.png)
![screenshot2](examples/screenshot2.png)
![screenshot3](examples/screenshot3.png)
![screenshot4](examples/screenshot4.png)
![screenshot5](examples/screenshot5.png)
![screenshot6](examples/screenshot6.png)
![screenshot7](examples/screenshot7.png)

---

[VirusTotal check](https://www.virustotal.com/gui/file/790c8e66aa1ef920f819445e8b439ae7a3da877279220f05eac885881ed25e54/detection)
![screenshot8](examples/screenshot8.png)

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
this project is also my first, fully finished C project, which i am quite proud of.  
(the actual exe is under 1 MB! but the SDL3 dll is almost 5 MBs...)

licensed under MIT. credits to microsoft for the retro windows icon (icon.ico).

# build

- SDL3 is included inside this repository, so just clone this repository
- you need vs c/c++ redistributable and gcc installed
- run `./build.sh`, and it will build everything for you with gcc inside `build/debug`. run with `./build/debug/plainIMG.exe` (or `./build.sh -a` for short)
- if youre not statically building with `-s`, you NEED to have `SDL3.dll` and `SDL3_image.dll` located in the same directory as the executable.
- build the resoures (icon + welcome text) with `windres src/plainIMG.rc -O coff -o src/plainIMG_rc.o`

libraries used: [SDL 3](https://github.com/libsdl-org/SDL), [SDL3_image 3](https://github.com/libsdl-org/SDL_image/),

if you want to update SDL or SLD_Image, you would have to build static binaries (`*.a`) yourself.  
just clone the repositores, make a `build` folder, inside it run `cmake .. -DSDL_STATIC=ON` and `cmake --build .`.  
then, just copy over generated `.a` files to `lib/` folder of each library.

for SDL3_image, there is a special case. copy over the entire SDL folder, where you just built it inside `build/`,  
then run SDL3_image build with `cmake .. -DBUILD-SHARED-LIBS=OFF`, then do `cmake --build .`.
you would also need to use `git submodule update --init --recursive` in SDL image.

if you want to make changes, then make your changes in `dev` branch, and sync them to main with `./sync.sh`.  
when you want to release, switch to `main`.

you need inno setup installed to build for release, and it should be in the PATH.  
when you want to build a new release, edit `VERSION.txt` with the version format being `vX.X.X`.  
to build all the things for release, run `./release.sh`. it will create `build/release`, where `build/release/upload` is the things you would need to upload to github release.  
mark your release with `./tag_release.sh`. it will automatically push it. **make sure that you correctly specify the version, and it's not a duplicate**

# todo list

- [ ] save menu configurations to some text file, so that it will save between launches
- [ ] add a reminder when there is a new version of this app
- [ ] finish up lite mode
- [x] add icon to the executable with windres
- [x] build this exe statically (basically you need to have .a's instead of .dll's)
- [ ] make a lite mode where the title bar will be hidden, so you will be able to see just the picture
- [ ] make proper version control (possibly with an auto-updater)
- [x] make an inno setup installer
- [ ] fix the client area dragging being a bit choppy
- [x] add a global VERSION.txt file which will be used everywhere (main.c, installer.iss, tag_release.sh etc)
- [ ] add the image basename when an image is opened to the window's title
- [ ] possibly make an .msi installer
- [ ] make the app notify you if it couldnt open an app instead of just silencing it
- [ ] fix the fact that sometimes, when you resize window to image, it just doesnt properly resize, and it keeps on resizing
