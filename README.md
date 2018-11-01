# BrhanRenderer - WIP Path Tracer

## Index
- [Build Instructions](#building)
  - [Linux](#linux)
  - [Windows](#windows)
- [Run Instructions](#running)
- [Scene File Documentation](#scene-file-documentation)
- [Supported Features](#functionality)
  - [BxDFs](#bxdfs)
  - [Materials](#materials)
  - [Textures](#textures)
  - [Misc](#misc)
- [Renders](#rendered-images)

## Building
Development has primarily been performed on a Linux Ubuntu 16.04 machine, but it should build on both Linux and Windows. Steps for both platforms are listet below. The project has one submodule dependency to my own memory pool implementation.
### Linux
```
git clone https://github.com/MulattoKid/BrhanRenderer.git 
cd BrhanRenderer
git submodule update --init --recursive
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
git submodule update --init --recursive
```
The only method detailed below is compiling using MSVC (**recommended**). Other methods such as using Cygwin or WSL are likely to work just fine, but I assume anyone wanting to use other methods know how to do so.

*setup.bat* is responsible for setting up the required environment variables so that ```cl``` (and friends) can be called directly from the working directory. If you installed Visual Studio 2017 in its default location, the script should work fine. If not, or you have a different Visual Studio version, edit the file to call ```vcvarsall.bat``` form its location on your system. E.g. ```call "your_install_directory\vcvarsall.bat" x64```.
#### Debug build
```
setup.bat
build_debug.bat
```
#### Release build
```
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
### BxDFs
- Lambertian BRDF
- Oren-Nayar BRDF
- Specular BRDF
- Specular BTDF
### Materials
- Matte
- Mirror
- Plastic (WIP)
- Water
- Glass
### Textures
- Matte, mirror and plastic materials composed of textures
- Automatic mipmap generation and selection
- Bilinear filtering
### Misc
- BVH tree for ray-shape intersection
- Memory pool for avoiding dynamic heap allocations while rendering, i.e. ```new```/```delete```
- OpenMP parallelization

## Rendered Images
![CornellBox](https://i.imgur.com/DGFBV83.png)
A view of the CornellBox-Original (model courtesy of Morgan McGuire's graphics archive) rendered with the following settings:
- 1024x1024
- 512 samples per pixel
- Max path depth of 4
- 19.5 degrees of vertical camera FOV

Triangle count: 36

Render time: 7m 36s using 12 cores

![Buddha statue](https://i.imgur.com/0GbiD1E.png)
A view of the Buddha statue (model courtesy of [Stanford Computer Graphics Laboratory](http://graphics.stanford.edu/data/3Dscanrep/)) rendered with the following settings:
- 1024x1024
- 256 samples per pixel
- Max path depth of 3
- 60 degrees of vertical camera FOV

Triangle count: 29892

Rendering time: 1m 33 using 12 cores
