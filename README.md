# BrhanRenderer - WIP Path Tracer

## Index
- [Build Instructions](#building)
  - [Linux](#linux)
  - [Windows](#windows)
- [Run Instructions](#running)
- [Scene File Documentation](#scene-file-documentation)
- [Supported Features](#functionality)
- [Renders](#rendered-images)

## Building
Development has primarily been performed on a Linux Ubuntu 16.04 machine, but it should build on both Linux and Windows. Steps for both platforms are listet below.
### Linux
```
git clone https://github.com/MulattoKid/BrhanRenderer.git
cd BrhanRenderer
mkdir build
```
#### Debug build
```
make debug -j8
```
#### Release build
```
make -j8
```

### Windows
```
git clone https://github.com/MulattoKid/BrhanRenderer.git
cd BrhanRenderer
```
The only method detailed below is compiling using MSVC (**recommended**). Other methods such as using Cygwin or WSL are likely to work just fine, but I assume anyone wanting to use other methods know how to do so.

*setup.bat* is responsible for setting up the required environment variables so that ```cl``` (and friends) can be called directly from the working directory. If you installed Visual Studio 2017 in its default location, the script should work fine. If not, or you have a different Visual Studio version, edit the file to call ```vcvarsall.bat``` form its location on your system. E.g. ```call "your_install_directory\vcvarsall.bat" x64```.
#### Debug build
```
mkdir build_debug
setup.bat
build_debug.bat
```
#### Release build
```
mkdir build
setup.bat
build.bat
```

## Running
This step is platform independent.
```
./build/BrhanRenderer path_to_scene_file
```
On both platforms, the rendered image is placed in the directory form which the call to BrhanRenderer was made. The ```scenes/```folder holds some default scenes that can be used to test the renderer. The models used for these scenes are located in ```models/```.

## Scene file documentation
TODO

## Functionality
TODO

## Rendered Images
![CornellBox](https://i.imgur.com/DGFBV83.png)
A view of the CornellBox-Original (model courtesy of Morgan McGuire's graphics archive) rendered with the following settings:
- 1024x1024
- 512 samples per pixel
- Max path depth of 4
- 63 degrees of vertical camera FOV

Triangle count: 36

Render time: 7m 36s using 11 cores

![Buddha statue](https://i.imgur.com/hQ3fVck.png)
A view of the Buddha statue (model courtesy of [Stanford Computer Graphics Laboratory](http://graphics.stanford.edu/data/3Dscanrep/)) rendered with the following settings:
- 1024x1024
- 256 samples per pixel
- Max path depth of 3
- 60 degrees of vertical camera FOV

Triangle count: 29892

Rendering time: 1m 55s using 11 cores
