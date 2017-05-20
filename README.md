# Exoplanet Simulator
Software and (soon) build instructions on an astrophysics demonstration to
show how we can detect exoplanets by DFT analysing how the distance of a star changes
with time.

## What's in this repository?
In this repository is code for a computer and arduino. The computer software is written
in QT c++ and the arduino software is written in normal arduino code. The arduino used
for testing is the DF Robot Bluno (Basically an uno) becuase I though a BLE connection
between the computer and controller would make wiring far easier, but the software can
cope with any type of arduino. Currently it assumes a 115200 baud rate but I plan to
remove this assumption soon so you don't have to recompile just to use it with another
arduino.
