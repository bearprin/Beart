## Beart

---

Beart is my personal Ray Tracing Render for studying. It is inspired by many open-source renderers.
The project is still under development.

### Building the project

Clone the repository and submodules:

```bash
$ git clone --recursive https://github.com/bearprin.git
$ git submodule update --init --recursive
```

Use CMake to generate the build system for your favorite IDE or compiler.
For example, on Mac/Linux, you can use the following commands to generate a Makefile and build the project:

```bash
$ cd beart
$ cmake -B build -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
$ cmake --build build -j
```

### Usage

Find the executable file in `build/bin` directory.

```bash
$ ./build/bin/beart -s scenes/{scenes_file_path} -o {output_file_path}
```

where -s is the scene file path, -o is the output file path.

The default output file path is the "output/{scenes_file_name}_{timestamp}.exr"

We recommend [tev](https://github.com/Tom94/tev) and [HDRView](https://github.com/wkjarosz/hdrview) to view the output
image.

### Features

#### SceneFile

- Load scene from file
- [ ] Blender exporter
- [ ] Mitsuba importer

#### Integration

- Direct Illumination
- Ambient Occlusion
- Path Tracing
    - [ ] VolPath Tracing
    - [ ] Modern Volume Rendering,
- Light Tracing
- Bidirectional Path Tracing (may be some bugs, fix later)
    - [ ] Light Transport Simulation with Vertex Connection and Merging, Georgiev, 2012
-[ ] Photon Mapping

#### Material

- Lambertian Diffuse
- [ ] Oran Nayar Diffuse
- Modified Phong model
- Microfacet model
    - conductor 
    - dielectric
    - [ ] Multiple-Scattering Microfacet BSDFs with the Smith Model, Heitz, 2016
    - [ ] Importance Sampling Microfacet-Based BSDFs using the
      Distribution of Visible Normals, Heitz, 2014

#### Camera

- Perspective Camera

#### Light

- Point Light
- Directional Light
- Area Light

#### Acceleration

- BVH
- [ ] KD-Tree

#### Sampler

- Random Sampler
- [ ] Stratified Sampler

[//]: # (### Image Gallery)

[//]: # ()
[//]: # (![buddha-box]&#40;gallery/buddha-box&#41;)



### Dependencies

- [assimp](https://github.com/assimp/assimp)
- [CLI11](https://github.com/CLIUtils/CLI11)
- [enoki](https://github.com/mitsuba-renderer/enoki)
- [nlohmann-json](https://github.com/nlohmann/json)
- [nanothread](https://github.com/mitsuba-renderer/nanothread)
- [oidn](https://github.com/OpenImageDenoise/oidn)
- [pcg-cpp](https://github.com/imneme/pcg-cpp)
- [spdlog](https://github.com/gabime/spdlog)
- [stb](https://github.com/nothings/stb)
- [tineyexr](https://github.com/syoyo/tinyexr)

### References

See [REFERENCES.md](REFERENCES.md) for a list of references.

### Acknowledgements
Since scene data is very important for the render, I have transferred some scene files from other open-source renderers.

I would like to thank the scenes files and models from [darts]("https://cs87-dartmouth.github.io/Fall2022/index.html"), [ecse689](https://www.cim.mcgill.ca/~derek/ecse689.html).
