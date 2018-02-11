# 1771-powerup-robot [![Build Status](https://travis-ci.org/jscd/1771-powerup-robot.svg?branch=master)](https://travis-ci.org/jscd/1771-powerup-robot)
This repo is the C++ code for FRC Team 1771's 2018 Power Up robot.

## Building

This was built on a macOS High Sierra system and a Debian 4.9.0-3-amd64 system.

If building on your machine run ```./gradlew installToolchain``` to install the FRC C++ toolchains.


To build locally run:
```
./gradlew build
```

## Deployment

To deploy to roborio run:
```
./gradlew deploy
```

## Built With

* [GradleRIO](https://github.com/Open-RIO/GradleRIO) - The C++ gradle framework used

## Authors

* **Justin DeSimpliciis** - *Initial work* - [jscd](https://github.com/jscd)
