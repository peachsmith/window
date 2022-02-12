# window
This is an example of creating a basic window using SDL2.
<br/>
This code was originally compiled with SDL version 2.0.18.
<br/>

## Building
The Makefile in the root directory of this repo is intended for Mac and Linux. For Windows, use the Makefile in the msvc directory.
### Mac and Linux
To build on Mac or Linux, install clang or gcc and update the `CC` variable in the Makefile to be the compiler of your choice. Then run `make` in the root directory of the repo.
### Windows
On Windows, use NMAKE from the MSVC build tools. This can be done by starting the x64 Native Tools Command Prompt and navigating to the `msvc` directory in the project. Then run `nmake`.

## Known Issues

### Keyboard Rollover
Different keyboards have different hardware limitations on simultaneous key presses. See https://en.wikipedia.org/wiki/Rollover_(keyboard). For example, when holding the left and right arrow keys, the space bar may not be detected until either the left or right arrow key is released. This can be different for different keyboards, and is out of the control of SDL.
