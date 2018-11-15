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
The scene file (.brhan extension, although the extension is not critical for a file to be used as one) describes the scene and the render settings. A few settings must be present for the renderer to be able to render anything. These are:
- ```Camera position[x y z] view_direction[x y z] vertical_fov[n] width[n] height[n]```

- ```Integrator type[path] spp[n] max_depth[n]```

Geometry specifications are provided like so:
- ```Model file[f.obj]```

- ```Sphere center[x y z] radius[n]```

OBJ files generally have a MTL file associated with them. In that case, no more information is needed and rendering can commence.  If the material for a piece of geometry isn't supported, the system will alert you of this.
However, if no MTL file is available, or you want to specify your own material or you provide the details of a sphere, it is *requried* to specify a material name and the needed reflection and/or transmittance values.

The supported materials with corresponding required details are:
- matte -> ```diffuse[r g b]```
- mirror -> ```specular[r g b]```
- plastic -> ```diffuse[r g b] specular[r g b]```
- aluminium -> ```specular[r g b]```
- copper -> ```specular[r g b]```
- gold -> ```specular[r g b]```
- salt -> ```specular[r g b]```
- water -> ```reflectance[r g b] transmittance[r g b]```
- glass -> ```reflectance[r g b] transmittance[r g b]```

**Note**: textures are not supported when specifying a custom material.

Additionally, any model or sphere can be translated, rotated and scaled by specifying the respective settings:
- ```translate[x y z]```
- ```rotate[x y z]```
- ```scale[x y z]```

Lastly, two settings are available to generate incremental images while rendering and to generate a depth image:
- ```SaveIntervals[n]```
- ```GenDepthImage[true/false]```

Here's an example of what a *scene.brhan* file can look like:
```
Camera position[0.0 1.0 5.58] view_direction[0.0 0.0 -1.0] vertical_fov[19.5] width[1024] height[1024]
Integrator type[path] spp[256] max_depth[4]
SaveIntervals[4]
GenDepthImage[true]

Model file[models/CornellBox/CornellBox-Empty-RG.obj] translate[0 0 -1]
Sphere center[0 0.5 0] radius[0.25] material[copper] specular[0.28 0.17 0.08]
```

## Functionality
### BxDFs
- Lambertian BRDF
- Oren-Nayar BRDF
- Specular BRDF
- Specular BTDF
- Microfacet BRDF using Beckmann's microfacet distribution function
### Materials
- Matte
- Mirror
- Plastic (WIP)
- Metals (Fresnel conductor): aluminium, copper, gold
- Water (Fresnel dielectric)
- Glass (Fresnel dielectric)
### Textures
- Matte, mirror and plastic materials composed of textures
- Automatic mipmap generation and selection
- Bilinear filtering
### Misc
- BVH tree for ray-shape intersection
- Writing out images during rendering (useful for generating image data for neural networks)
- Memory pool for avoiding dynamic heap allocations while rendering, i.e. ```new```/```delete```
- OpenMP parallelization

## Rendered Images
![CornellBox with glass sphere](https://i.imgur.com/xI2jXSe.png)
A view of the CornellBox-Original (model courtesy of Morgan McGuire's graphics archive) with an added glass sphere rendered with the following settings:
- 1024x1024
- 2048 samples per pixel
- Max path depth of 4
- 19.5 degrees of vertical camera FOV

Triangle count: 36
Sphere count: 1

Render time: 29m 48s using 12 cores


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
