# xshader
A simple application to run GLSL shader programs as the root window on systems using X.

# Usage
1. Clone the repo `git clone https://github.com/lord-123/xshader.git`
2. `CD` into the directory.
3. Configure the program by modifying `config.h` if you so wish.
   * `shaderPath` is the location of the fragment shader program, a few examples can be located in [/shaders](/shaders). Note it must be an absolute directory (no `~/`). This may be changed in the future.
   * `refreshRate` is how often the program will be delayed between redrawing the screen. Having this low can be a serious power draw on laptops so watch out.
4. Run `sudo make clean install`.
5. Add `xshader &` to your `.xinitrc`.
6. Logout and log back in to update the background.
