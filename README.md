# OtoDecks

This is an endterm project for the course "Object Oriented Programming" at the University of London.

## Compiling

To successfully work with this code you need to have the following installed:

* JUCE v7.0.5
* C++17 compatible compiler
* [FRUT](https://github.com/McMartin/FRUT) to build the Projucer CMake files on macOS.
* CMake 3.10 or higher

To compile the project on macOS, install [FRUT](https://github.com/McMartin/FRUT) in the same directory as this project and run the following commands to generate the CMake files:

```bash
../FRUT/prefix/FRUT/bin/Jucer2CMake reprojucer OtoDecks.jucer ../FRUT/prefix/FRUT/cmake/Reprojucer.cmake
```

You should run the FRUT command each time after you added a new file into the project.

Then you can build the project with CMake:

```bash
mkdir build
cd build
cmake ..
make
```

If you have the following error:

```
error: "This project was last saved using an outdated version of the Projucer! Re-save this project with the latest version to fix this error."
```

Please open the project in Projucer and save it again.


## Running

To run this project on macOS:

```bash
open OtoDecks.app
```

## Create an archive of the project

```bash
git archive --format=zip  --output=../OtoDecks_v1.zip main .
```

## References
* [1] Initial code was taken from the University of London's Object Oriented Programming course.
* [2] [JUCE](https://juce.com/)
* [3] [FRUT](https://github.com/McMartin/FRUT)
* [4] [CMake](https://cmake.org/)
* [5] [Icons](https://www.flaticon.com/authors/pixel-perfect/lineal-color?author_id=168&type=standard) made by [Pixel perfect](https://www.flaticon.com/authors/pixel-perfect) from [www.flaticon.com](http://www.flaticon.com/)
