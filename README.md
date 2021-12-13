# codeChallenge
A C++ coding challenge requested by a potential employer

# Description
This is a console application which instructs a robot to move around a 5x5 grid. It can have it's position and rotation changed via user input.

# Constraints
 - Robot cannot fall off 5x5 table top
 - Robot cannot process any COMMANDS if it is not on the table
 - All input must be handled gracefully
 - Application must include test data


# Application includes
 - C++ Unit tests
 - Import of test data via .txt file
 - Functional application for user input

# How to build and run application
 1. Clone project and navigate to the directory
 2. Create **build** folder in directory ```cmake -S . -B build```
 3. Build project ```cmake --build build```
 4. Navigate to **build** folder ```cd build```
 5. Run unit tests by adding a console arg of *"0"* ```./ToyRobotCodeChallenge 0```
 6. Run test data by adding a console arg of "1" and a *path* ```./ToyRobotCodeChallenge 1 ../testData.txt```

# Commands
- ```PLACE x,y,rotation``` places robot on tabletop at position(x,y) with rotation
- ```MOVE```    moves robot in the direction it is facing
- ```LEFT```    rotates robot to the left direction by 90deg
- ```RIGHT```   rotates robot to the right direction by 90deg
- ```REPORT```  outputs the position and rotation of the robot

# EXAMPLE user input and expected output
```
PLACE 0,0,NORTH
MOVE
RIGHT
MOVE
LEFT
REPORT
Output: 1,1,NORTH
```


