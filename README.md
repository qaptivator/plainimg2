![plainIMG banner](examples/plainIMG_banner2.png)

# [plainIMG2 (plainIMG)](https://github.com/qaptivator/plainimg2)

_as simple as it gets for an image viewer_

plainIMG is a distraction-free **image viewer**.<br />
no toolbars, **no clutter**, no AI -- just the image.<br />
inspired by PowerToys "Always on Top", it's perfect for **pinning** reference images or artworks.<br />
also made to deshittify Windows and embrace FOSS -- fast, lightweight, and **user-first**.

# why would i use this?

1. **blazing fast:** opens images almost twice as fast as Photos app.

![plainIMG_open_comparison](examples/plainIMG_open_comparison.gif)

2. **always on top:** opened images are pinned by default, no extra software needed.
3. **lightweight:** the executable is under 3MB!

![plainIMG size](examples/plainIMG_size.png)

4. **smart window draging:** you can drag the window around using the actual window (the client area) instead of just the title bar!
5. **pixel-perfect features:** toggle antialiasing, stretch images, and more. ideal for pixel art.

# who is this for?

- developers wanting to pin important info or graphs during development
- students wanting to pin cheatsheets or charts during study
- pixel art artists, working with low resolution images

# usage

- **install the installer from the latest release. (`plainIMG_vX.Y.Z_installer.exe`)**
- run it and it will install the app, and also add plainIMG to start menu. if you click the "Set plainIMG as the default image viewer" option, jpg, jpeg, png and bmp file extensions will be associated with plainIMG.
- now that it is installed, you can open an image, select plainIMG and click "Always".
- alternatively, you can go to settings, default apps, and set common image formats to plainIMG.
- when you are done, you can use plainIMG by clicking on the image (or using "Choose an App on your PC")
  - or by running the executable and opening the picture with `Right Click > Open Image...` (or just letter `O`)
  - or by running it through the terminal with first argument being the file path
  - or by dragging a picture over the executable

the main menu is opened with `Right Click`.
you can close the window with `Right Click > Quit`, `Q`, `ESCAPE`, or just by closing the window.
to make the window be the size of the image, press `R`.
to make the window be over other windows and also dont go behind other windows when unfocused, press `T`. _this is enabled by default._  
to see the rest of the features, look down below.  
to support the developer, you can appreciate the project by giving it a star in the GitHub repository. thanks!

# hotkeys

- `Right Click` → menu
- `Q` / `ESC` / close window → quit
- `R` → resize window to image
- `T` → toggle always-on-top (default ON)
- `A` → maintain aspect ratio (default ON)
- `B` → black background for PNGs
- `L` → toggle antialiasing

# all of the features

- open an image using in-app file selector with `O`,
  - or give it the image path as first cli argument,
  - or as a hidden windows feature, drag an image over the executable and it will consider it as the first cli argument.
- close an image with `C`, which puts you back into the starting menu.
- open actions menu with `Right Click`, which shows all of the actions which you can do and their corresponding hotkeys.
- make the windows stay on top of other windows using `T`. this basically pins the window, and it wont disappear when you unfocus out of the window. enabled by default.
- use a black background instead of white one with `B`. useful for png files. disabled by default.
- keep the aspect ratio of the image with `A`. if you disable this, it will stretch the picture across the entire window. enabled by default.
- resize the window to be the image's size with `R`. note: only works if keep aspect ratio is enabled.
- when working with small resolution images, antialiasing usually ruins the image, so disable it with `L`. antialiasing is enabled by default.
- the window is also draggable in client area (the area other than the title bar), but be aware that native windows functions like window snapping wont work here. they only work with the title bar provided by windows.
- quit the app by closing the window, pressing `Q` or `ESCAPE`.
- the link to this github repository is located inside the `About` menu.
- WARNING: as of now, this project is only available on Windows. if this project gets enough attention, i will make a version for macOS and possibly even Linux.
- note: when you click the "Set plainIMG as the default image viewer" option in the installer, it automatically sets plainIMG as default for: .jpg, .jpeg, .png and .bmp; if you want to extend this list, update file extension defaults yourself in windows settings.
- TIFF, AVIF and WEBP are not supported.
- animated GIFs are not supported.
- your settings (keepAspectRatio useBlackBg useAntialiasing alwaysOnTop keepWindowAspectRatio showImageName) are stored inside `settings.txt`. i serialize these bools into string of 0's and 1's, where 0 is false, 1 is true. the order of those numbers matters here. the order was specified at the start of this point. anything other than 0 or 1 is considered a comment and just ignored.

# screenshots

start screen

![screenshot1](examples/screenshot1.png)

in black background mode

![screenshot2](examples/screenshot2.png)

opened image

![screenshot3](examples/screenshot3.png)

when you keep image aspect ratio, with black background

![screenshot4](examples/screenshot4.png)

when you DONT keep image aspect ratio, it stretches the image to the window size like this

![screenshot5](examples/screenshot5.png)

disable antialiasing so you can look at crispy pixel art textures without all the blur

![screenshot6](examples/screenshot6.png)

the right click menu as of v1.2.4

![screenshot7](examples/screenshot7.png)

---

[VirusTotal check (as of older versions)](https://www.virustotal.com/gui/file/790c8e66aa1ef920f819445e8b439ae7a3da877279220f05eac885881ed25e54/detection)
![screenshot8](examples/screenshot8.png)

[VirusTotal check (as of v1.3.2)](https://www.virustotal.com/gui/file/fdb7a5cc926a939aeb6293e0c93b3ec93154eb9b28089cf5851c5c5e6bdc4723/detection)

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

# todo list

- [x] [[[--MAIN THINGS--]]]
- [ ] add "copy image to clipboard" and "paste image from clipboard" (TOP PRIORITY because it is most useful)
- [ ] add folder exploration
- [ ] add image zooming
- [ ] add text rendering over the image, so you can make simple text overlays
- [ ] add support for animated GIFs using [IMG_LoadAnimation](https://wiki.libsdl.org/SDL3_image/IMG_LoadAnimation)
- [ ] add support for WEBP (probably never)
- [ ] add a reminder when there is a new version of this app (possibly with an auto-updater)

- [x] [[[--OTHER THINGS (may be outdated)--]]]
- [ ] fix the fact that sometimes, when you resize window to image, it just doesnt properly resize, and it keeps on resizing
- [ ] make a lite mode where the title bar will be hidden, so you will be able to see just the picture
- [ ] fix the client area dragging being a bit choppy (probably isnt a bug because its barely noticeable, and its probably not fixable)
- [ ] possibly make an .msi installer (probably not, its extremely complicated for C)
- [x] make an inno setup installer
- [x] add icon to the executable with windres
- [x] build this exe statically (basically you need to have .a's instead of .dll's)
- [x] add a global VERSION.txt file which will be used everywhere (main.c, installer.iss, tag_release.sh etc)
- [x] add the image basename when an image is opened to the window's title
- [x] make the app notify you if it couldnt open an image instead of just silencing it
- [x] optimize the app so that it wont rerender every time, even though there are no changes
- [x] reduce the cpu usage spike when dragging or resizing the window
- [x] save menu configurations to some text file, so that it will save between launches
- [ ] possibly a bug: when you open an image and "keep image aspect ratio" is not turned on while "keep window aspect ratio" IS turned on, the image stretches to the default window size so the window aspect ratio is wrong. i wanted to fix this at first, not knowing that i can fix this by just modifying the settings, but even then i thought that making a whole minor update just to fix this one bug is just too much. so this will remain like this for now.
- [ ] possibly add window opacity integer input so you can layer images over other apps (to implement, use a built in SDL function, and for integrr input use windows api. the opacity will be displayed in the window title only if it's different from 100%, and displayed like `...[WK] [80%] "blah.png"`)
- [ ] add support for transparent pngs LITERALLY being transparent, basically the above feature but masked (which makes this much, MUCH harder to implement). this will also require INTEGERS to be stored in my 0/1 format, not nice
- [ ] add "show metadata" popup which shows basic info like resolution (VERY USEFUL for something like pixel art. or maybe you could put it in the window name near the filename...), lens information and so on. no metadata editing though, this app is all read only
- [ ] maybe, just maybe, listen for file changes and update the preview accordingly (will obviously be an option in the menu too)
- [ ] hide the "No image was selected!" wanring, it's just annoying. usually, when the user doesnt select anything, they probably wanted to just close the browse window, OR they clicked open image accidentally

# build

- SDL3 is included inside this repository, so just clone this repository
- you need `vs c/c++ redistributable` and `gcc` installed
- run `./build.sh`, and it will build everything for you with gcc inside `build/debug`. run with `./build/debug/plainIMG.exe` (or `./build.sh -a` for short)
- if youre not statically building with `-s`, you NEED to have `SDL3.dll` and `SDL3_image.dll` located in the same directory as the executable.
- build the resoures (icon + welcome text) with `windres src/plainIMG.rc -O coff -o src/plainIMG_rc.o`

libraries used: [SDL 3](https://github.com/libsdl-org/SDL), [SDL3_image 3](https://github.com/libsdl-org/SDL_image/),

## build the installer

you need inno setup installed to build for release, and it should be in the PATH.  
when you want to build a new release, edit `VERSION.txt` with the version format being `vX.X.X`.  
to build all the things for release, run `./release.sh`. it will create `build/release`, where `build/release/upload` is the things you would need to upload to github release.  
mark your release with `./tag_release.sh`. it will automatically push it. **make sure that you correctly specify the version, and it's not a duplicate**

## build SDL3 and SDL_image manually

if you want to update SDL or SLD*Image, you would have to build static binaries (`*.a`) yourself.  
\_this section wont be specified in great detail, and wont be kept up to date, just because many libraries handle static building in their own way, and its really painful to figure them all out and write them down here.\*

### build SDL3

```sh
git clone https://github.com/libsdl-org/SDL.git && cd SDL
mkdir build && cd build
cmake .. -DSDL_STATIC=ON
cmake --build .
```

after that it will make a `libSDL3.a`. copy it to `plainimg2/src/SLD3/lib`.

### build SDL_image

```sh
git clone https://github.com/libsdl-org/SDL_image.git && cd SDL_image
git clone https://github.com/libsdl-org/SDL.git # SDL_image requires SDL as a dependency
git submodule update --init --recursive
mkdir build && cd build
cmake .. -DBUILD-SHARED-LIBS=OFF
cmake --build .
```

after that it will make a `libSDL_image.a`. copy it to `plainimg2/src/SDL3_image/lib`.  
SDL_image will also give errors where certain libraries are not found.  
these libraries are required for other file formats like WebP to work.  
i tried building them myself statically but its just very painful to do.  
that is why im not planning on adding support to WebP, TIFF and other unsupported formats.
