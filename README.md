# raytracer

Implements a raytracer based on the book [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html). 

## Final Images

![ref](/images/all-reflect.png)

![fin](/images/final.png)

![fin1](/images/final2.png)

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
raytracer $ mkdir build
raytracer $ cd build
raytracer/build $ cmake -G "Visual Studio 16 2019" ..
raytracer/build $ start CS312-Raytracer.sln
```

Your solution file should contain multiple projects, such as `gradient` and `normals`.
To run from the git bash command shell, 

```
raytracer/build $ ../bin/Debug/gradient
raytracer/build $ ../bin/Debug/normals
```

*macOS*

Open terminal to the directory containing this repository.

```
raytracer $ mkdir build
raytracer $ cd build
raytracer/build $ cmake ..
raytracer/build $ make
```

To run each program from build, you would type

```
raytracer/build $ ../bin/gradient
raytracer/build $ ../bin/normals
```

## Depth of Field

Box:
Takes a point, 3 normals, 3 halflengths, and a material and creates a parallelagram centered at the point. 

![box](/images/box.png)

Plane:
Takes a point, a normal, and a material and creates a plane through the point perpendicular to the normal.

![plane](/images/plane.png)

Depth of Field:

The degree of blur is determined by the radius (no blur if it's 0) and the sharpest point is at the focal length. The default radius is 0.

![depth](/images/depth.png)



