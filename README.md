# My Own 3D engine

> A simple dumb 3D engine for my own game :)

This engine is written in pure C/OpenGL with minimal use of third-party libraries

## Features 
- **One binary** – all resources inside, no external files
- **Minimalist** – just what you need for 3D rendering
- **Fullscreen mode** - automatically adjusts to the monitor
- **Camera Control** – WASD + Mouse (in development)
- **Loading models** – OBJ support via native converter [obj2h](https://github.com/K0nS0ller/obj2h)

## Build
First, you need to install the glfw library. You can do this by downloading it from the [website](https://www.glfw.org) or from your distribution's repository:


# Ubuntu / Debian
```bash
sudo apt install build-essential libglfw3-dev libgl1-mesa-dev imagemagick
```
# Arch Linux
```bash
sudo pacman -S gcc glfw-wayland mesa imagemagick
```
then compile:
```bash
chmod +x build.sh
./build.sh
```
then just run the binary



## Controls
  --- WASD -  camera movement ---
  
  --- Mouse - camera look ---
  
  --- ESC - exit ---
