# cavity_EM_waves

Simulation and animation engine for electromagnetic (EM) waves confined in a 2D rectangular cavity with totally reflecting walls.

requered library for compilaton (GNU/Linux):

1) [armadillo](http://arma.sourceforge.net/) C++ linear algebra library

2) [OpenCV](http://opencv.org/)

3) g++

how to compile (GNU/Linux):
```
g++ waves.cpp -o waves `pkg-config --cflags --libs opencv`
```
usage example (GNU/Linux):
```
./waves 6 1280 720 24 0.1
```
this creates a 1280x720 video @ 24 frames per second of the simulated wave with cavity normal modes up to order 6. The last argument "0.1" indicates the speed scale. 
