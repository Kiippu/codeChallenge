#include <stdio.h>
#include <cstring>
#include <iostream>

#include "Objects.h"
#include "UnitTests.h"

/**
 * TODO:
 *  - comments
 *  -  git push 
 * ls:          iress/ToyRobotCodeChallenge/build
 * dataSet:     ./ToyRobotCodeChallenge 1 testData.txt
 * unitTests:   ./ToyRobotCodeChallenge 0
 * userInput:   ./ToyRobotCodeChallenge
 * */

/**
 * @brief main runtime loop
 * @param argc amount of arguments agaliable
 * @param argv array of char arguments
*/
int main(int argc, char *argv[]) 
{
    // create base object states
    const object::TableTop tableTop(TABLE_TOP_X, TABLE_TOP_Y);
    // get player robot from map
    object::ToyRobot& player = tableTop.getPlayer();
    // run tests
    if(argc > 1)
    {
        UnitTests unitTests(tableTop);
        // run dataset tests
        if(strcmp(argv[1],"1")==0)
        {
            printf("run text test\n");
            unitTests.runDataSetTests(argv[2]);
            return 0;
        }
        // run unit tests
        else
        {
           unitTests.runUnitTests();
        }
    }
    // run user input
    else
    {
        while(true)
        {
            // get input from user
            std::string input;
            std::getline( std::cin, input);
            // proccess used input
            player.proccessInput(input);
        }
    }
    return 0;
}
