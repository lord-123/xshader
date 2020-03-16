# xshader
A simple application to run GLSL shader programs as the root window on systems using X.

# Examples
The window manager used in these screenshots is [DWM](https://dwm.suckless.org/) and I am using [Luke Smith's dwmblocks build](https://github.com/LukeSmithxyz/dwmblocks) to set the status with some custom scripts.
![Gradient shader](/resources/gradient.png)
*Using the [`gradient.frag`](/shaders/gradient.frag) shader program*

# Usage
1. Clone the repo `git clone https://github.com/lord-123/xshader.git`
2. `cd` into the directory.
3. Configure the program by modifying `config.h` if you so wish.
4. Run `sudo make clean install`.
5. Add `xshader &` to your `.xinitrc`.
6. Logout and log back in to update the background.

# Configuring `config.h` for custom shaders
* `SHADER_PATH` is the location of the fragment shader program (note: by default the example [`/shaders`](/shaders) directory is copied to `~/.local/bin/shaders`). It must be an absolute directory (no `~/`). This may be changed in the future.
* `DELAY` is how often the program will be delayed between redrawing the screen. Having this low can be a serious power draw on laptops so watch out.
* The number of shader uniforms used MUST be set correctly in `UNIFORM_COUNT`. This may not be necessary in a future update.
* The custom uniforms are defined by their name and a custom updater function.

# Features
## Current
* Custom handwritten shaders
* Completely customizable
* Written in pure C for maximum efficiency and minimal power consumption
* Supports custom uniforms which update each frame (animated wallpapers)
## TODO
* One time shaders at the start of the program
* Seperate shader update rates

# Contributing
I would be very grateful for anybody wanting to contribute anything. Issues and pull requests would be excellent, however, if you would like a substantial feature, I might suggest creating a different fork as I am trying to keep this as minimal as possible. Custom shaders from people would be a great addition to this project.
