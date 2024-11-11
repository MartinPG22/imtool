#!/bin/sh 
# build.sh 
cmake -S PracticaArcos2 -B PracticaArcos2-release -DCMAKE_BUILD_TYPE=release
cmake --build PractiaArcos2-release
