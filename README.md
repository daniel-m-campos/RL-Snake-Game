# RL Snake Game
<img src="snake_game.gif"/>

This is an Reinforcement Learning Snake Game with an NCurses UI for selecting to either:
1. Play snake yourself.
1. Watch an RL bot play.
1. Tran a new RL bot to play, which you can then watch.

The game code for this repo was inspired by [CppND-Capstone-Snake-Game](https://github.com/udacity/CppND-Capstone-Snake-Game).

## Dependencies for Running Locally
* cmake >= 3.13
* All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac)
    * Linux: make is installed by default on most Linux distros
    * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
* SDL2 >= 2.0
    * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
    * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* NCurses >= 6.1
* gcc/g++ >= 9.3
    * Linux: gcc / g++ is installed by default on most Linux distros
    * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)

### Installing/Upgrading dependencies on Linux
```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install libncurses5-dev libncursesw5-dev
sudo apt install -y gcc-9 g++-9
export CC=gcc-9
export CXX=g++-9
```

## Basic Build Instructions
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && cmake --build .`
4. Run it: `./RLSnakeGame`.