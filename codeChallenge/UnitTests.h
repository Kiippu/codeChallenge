#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

#include "Objects.h"

#include <chrono>
#include <memory>
#include <sstream>
#include <string>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>


/**
 * @brief run euality check on int variables
 * @param _a int a to compare
 * @param _b int b to compare
 * @param _isCritical whether a fail/assert needs to occur
*/
#define ASSERT_EQUALS_INT( _a, _b, _isCritical) { \
    if(_a != _b) {\
        printf("\n\nTEST: FAIL\t %s != %s - %i is not equal to %i",#_a,#_b,_a,_b);\
        if(_isCritical) assert(_a == _b && "A is not equal to B");}  \
    else printf("\nTEST: PASS\t - %s{%i} == %s{%i}",#_a,_a,#_b,_b);}

/**
 * @brief run euality check on string variables
 * @param _a string a to compare
 * @param _b string b to compare
 * @param _isCritical whether a fail/assert needs to occur
*/
#define ASSERT_EQUALS_STRING( _a, _b, _isCritical) { \
    if(_a != _b) { \
        printf("\nTEST: FAIL\t - %s == %s - %s is not equal to %s",_a.c_str(),_b.c_str(),_a.c_str(),_b.c_str());\
        if(_isCritical) assert(_a == _b && "A is not more then B");} \
    else printf("\nTEST: PASS\t - %s == %s",_a.c_str(),_b.c_str());}
   
/**
 * @brief creates test logging, profiling and exeption handling around a predefind test 
 * @param _test lambda tests of [&]()->void{}
*/ 
#define CREATE_TEST(_test) {\
    printf("\n%s - BEGINING TEST..",#_test);\
    const auto start(std::chrono::steady_clock::now());\
    try{\
        _test();\
    }\
    catch(const std::exception& e){\
        std::cerr << "\nTEST EXCEPTION: " << e.what() << '\n';\
    }\
    const auto end(std::chrono::steady_clock::now());\
    const auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();\
    const auto duration_ns = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();\
    printf("\nDURATION: : %lu ms , %lu µs", duration_ms, duration_ns);\
    printf("\n%s - FINISHED\n",#_test);\
}


class UnitTests
{
private:
    /// global tableTop
    const object::TableTop& m_tableTop;
public:

    UnitTests(const object::TableTop& _tableTop)
        : m_tableTop(_tableTop) 
    {}

    /**
     * @brief run an external data set .txt file
     * @param _path a path to find .txt file
    */
    void runDataSetTests(std::string _path = "./testData.txt")
    {
        // attempot to get file from _path
        std::ifstream file(_path);
        std::string data = "";
        std::string str; 
        // process data set per command/action
        printf("\nTEST:  processing data-set from file:%s\n",_path.c_str());
        while(getline(file, data,'|'))
        {
            printf("%s\n",data.c_str());
            object::ToyRobot& robot = m_tableTop.getPlayer();
            robot.proccessInput(data);
        }
        printf("TEST: data-set from file:%s COMPLETE\n",_path.c_str());
        
        file.close();
    }

    /**
     * @brief run all unit tests
    */
    void runUnitTests()
    {
        /**
         * TEST: toyRobot is the same one each access
        */
        auto test_toyRobot_address = [&](){

            std::ostringstream oss;
            oss  << &m_tableTop.getPlayer();
            std::string address_0 = oss.str();
            oss.str("");

            oss  << &m_tableTop.getPlayer();
            std::string address_1 = oss.str();
            ASSERT_EQUALS_STRING(address_0, address_1, true )
        };
        CREATE_TEST(test_toyRobot_address)

        /**
         * TEST: toyRobot actions should not register if the robot has not been placed
        */
        auto test_toyRobot_commands = [&](){
            object::ToyRobot& robot = m_tableTop.getPlayer();
            auto position = robot.getPosition();
            auto rotation = robot.getRotation();
            // move before place
            robot.move();
            auto afterPosition = robot.getPosition();
            auto afterRotation = robot.getRotation();
            ASSERT_EQUALS_INT(position.x, afterPosition.x, true);
            ASSERT_EQUALS_INT(position.y, afterPosition.y, true);
            ASSERT_EQUALS_INT(rotation, afterRotation, true);
            // left before place
            robot.rotateLeft();
            afterPosition = robot.getPosition();
            afterRotation = robot.getRotation();
            ASSERT_EQUALS_INT(position.x, afterPosition.x, true);
            ASSERT_EQUALS_INT(position.y, afterPosition.y, true);
            ASSERT_EQUALS_INT(rotation, afterRotation, true);
            // // right before place
            robot.rotateRight();
            afterPosition = robot.getPosition();
            afterRotation = robot.getRotation();
            ASSERT_EQUALS_INT(position.x, afterPosition.x, true);
            ASSERT_EQUALS_INT(position.y, afterPosition.y, true);
            ASSERT_EQUALS_INT(rotation, afterRotation, true);
            // // place before place
            // robot.rotateRight();
            afterPosition = robot.getPosition();
            afterRotation = robot.getRotation();
            ASSERT_EQUALS_INT(position.x, afterPosition.x, true);
            ASSERT_EQUALS_INT(position.y, afterPosition.y, true);
            ASSERT_EQUALS_INT(rotation, afterRotation, true);
        };
        CREATE_TEST(test_toyRobot_commands)

        /**
         * TEST: toyRobot PLACE command should relocate robot if it is within tabletop
        */
        auto test_toyRobot_placement = [&](){
            object::ToyRobot& robot = m_tableTop.getPlayer();

            // place robot at position {0,0,NORTH}
            // EXPECTATION: transform data does not change
            robot.placeHere(0,0,type::HEADING::NORTH);
            auto position = robot.getPosition();
            auto rotation = robot.getRotation();
            ASSERT_EQUALS_INT(position.x, 0, true);
            ASSERT_EQUALS_INT(position.y, 0, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::NORTH, true);

            // place robot at position {200,1,NORTH}
            // EXPECTATION: transform data does not change
            uint8_t PARAM_X = 200;
            uint8_t PARAM_Y = 1;
            robot.placeHere( PARAM_X, PARAM_Y,type::HEADING::NORTH);
            position = robot.getPosition();
            rotation = robot.getRotation();
            ASSERT_EQUALS_INT(position.x, 0, true);
            ASSERT_EQUALS_INT(position.y, 0, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::NORTH, true);

            // place robot at position {1,200,NORTH}
            // EXPECTATION: transform data does not change
            PARAM_X = 1;
            PARAM_Y = 200;
            robot.placeHere( PARAM_X, PARAM_Y, type::HEADING::NORTH);
            position = robot.getPosition();
            rotation = robot.getRotation();
            ASSERT_EQUALS_INT(position.x, 0, true);
            ASSERT_EQUALS_INT(position.y, 0, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::NORTH, true);

            // place robot at position {1,1,UNDEFINED}
            // EXPECTATION: transform data does not change
            PARAM_X = 1;
            PARAM_Y = 1;
            robot.placeHere( PARAM_X, PARAM_Y, type::HEADING::UNDEFINED);
            position = robot.getPosition();
            rotation = robot.getRotation();
            ASSERT_EQUALS_INT(position.x, 0, true);
            ASSERT_EQUALS_INT(position.y, 0, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::NORTH, true);

            // place robot at position {1,1,WEST}
            // EXPECTATION: transform data does change
            PARAM_X = 1;
            PARAM_Y = 1;
            robot.placeHere( PARAM_X, PARAM_Y, type::HEADING::WEST);
            position = robot.getPosition();
            rotation = robot.getRotation();
            ASSERT_EQUALS_INT(position.x, 1, true);
            ASSERT_EQUALS_INT(position.y, 1, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::WEST, true);
        };
        CREATE_TEST(test_toyRobot_placement)

        /**
         * TEST: toyRobot MOVE command should move robot 1 unit if it is within tabletop
        */
        auto test_toyRobot_movement = [&](){
            object::ToyRobot& robot = m_tableTop.getPlayer();
            uint8_t PARAM_X = 0;
            uint8_t PARAM_Y = 0;

            // place robot at position {0,0,WEST}
            // EXPECTATION: transform data does not change
            robot.placeHere( PARAM_X, PARAM_Y,type::HEADING::WEST);
            auto position = robot.getPosition();
            auto rotation = robot.getRotation();
            robot.move();
            ASSERT_EQUALS_INT(position.x, PARAM_X, true);
            ASSERT_EQUALS_INT(position.y, PARAM_Y, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::WEST, true);

            // place robot at position {0,0,SOUTH}
            // EXPECTATION: transform data does not change
            robot.placeHere( PARAM_X, PARAM_Y, type::HEADING::SOUTH);
            position = robot.getPosition();
            rotation = robot.getRotation();
            robot.move();
            ASSERT_EQUALS_INT(position.x, PARAM_X, true);
            ASSERT_EQUALS_INT(position.y, PARAM_Y, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::SOUTH, true);

            // place robot at position {4,4,WEST}
            // EXPECTATION: transform data does not change
            PARAM_X = 4;
            PARAM_Y = 4;
            robot.placeHere( PARAM_X, PARAM_Y, type::HEADING::WEST);
            position = robot.getPosition();
            rotation = robot.getRotation();
            robot.move();
            ASSERT_EQUALS_INT(position.x, PARAM_X, true);
            ASSERT_EQUALS_INT(position.y, PARAM_Y, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::WEST, true);

            // place robot at position {4,4,NORTH}
            // EXPECTATION: transform data does not change
            robot.placeHere( PARAM_X, PARAM_Y, type::HEADING::NORTH);
            position = robot.getPosition();
            rotation = robot.getRotation();
            robot.move();
            ASSERT_EQUALS_INT(position.x, PARAM_X, true);
            ASSERT_EQUALS_INT(position.y, PARAM_Y, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::NORTH, true);

            // place robot at position {0,4,EAST}
            // EXPECTATION: transform data does not change
            PARAM_X = 0;
            PARAM_Y = 4;
            robot.placeHere( PARAM_X, PARAM_Y, type::HEADING::EAST);
            position = robot.getPosition();
            rotation = robot.getRotation();
            robot.move();
            ASSERT_EQUALS_INT(position.x, PARAM_X, true);
            ASSERT_EQUALS_INT(position.y, PARAM_Y, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::EAST, true);

            // place robot at position {0,4,NORTH}
            // EXPECTATION: transform data does not change
            robot.placeHere( PARAM_X, PARAM_Y, type::HEADING::NORTH);
            position = robot.getPosition();
            rotation = robot.getRotation();
            robot.move();
            ASSERT_EQUALS_INT(position.x, PARAM_X, true);
            ASSERT_EQUALS_INT(position.y, PARAM_Y, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::NORTH, true);

            // place robot at position {4,0,WEST}
            // EXPECTATION: transform data does not change
            PARAM_X = 4;
            PARAM_Y = 0;
            robot.placeHere( PARAM_X, PARAM_Y, type::HEADING::WEST);
            position = robot.getPosition();
            rotation = robot.getRotation();
            robot.move();
            ASSERT_EQUALS_INT(position.x, PARAM_X, true);
            ASSERT_EQUALS_INT(position.y, PARAM_Y, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::WEST, true);

            // place robot at position {4,0,SOUTH}
            // EXPECTATION: transform data does not change
            robot.placeHere( PARAM_X, PARAM_Y, type::HEADING::SOUTH);
            position = robot.getPosition();
            rotation = robot.getRotation();
            robot.move();
            ASSERT_EQUALS_INT(position.x, PARAM_X, true);
            ASSERT_EQUALS_INT(position.y, PARAM_Y, true);
            ASSERT_EQUALS_INT(rotation, type::HEADING::SOUTH, true);
        };
        CREATE_TEST(test_toyRobot_movement)

        /**
         * TEST: toyRobot rotate LEFT command should rotate robot
        */
        auto test_toyRobot_rotate_left = [&](){
            object::ToyRobot& robot = m_tableTop.getPlayer();
            uint8_t PARAM_X = 0;
            uint8_t PARAM_Y = 0;

            // place robot at position {0,0,NORTH}
            // EXPECTATION: rotation equals WEST
            robot.placeHere( PARAM_X, PARAM_Y,type::HEADING::NORTH);
            robot.rotateLeft();
            auto rotation = robot.getRotation();
            ASSERT_EQUALS_INT(rotation, type::HEADING::WEST, true);

            // EXPECTATION: rotation equals SOUTH
            robot.rotateLeft();
            rotation = robot.getRotation();
            ASSERT_EQUALS_INT(rotation, type::HEADING::SOUTH, true);

            // EXPECTATION: rotation equals EAST
            robot.rotateLeft();
            rotation = robot.getRotation();
            ASSERT_EQUALS_INT(rotation, type::HEADING::EAST, true);

            // EXPECTATION: rotation equals NORTH
            robot.rotateLeft();
            rotation = robot.getRotation();
            ASSERT_EQUALS_INT(rotation, type::HEADING::NORTH, true);
        };
        CREATE_TEST(test_toyRobot_rotate_left)

        /**
         * TEST: toyRobot rotate RIGHT command should rotate robot
        */
        auto test_toyRobot_rotate_right = [&](){
            object::ToyRobot& robot = m_tableTop.getPlayer();
            uint8_t PARAM_X = 0;
            uint8_t PARAM_Y = 0;

            // place robot at position {0,0,NORTH}
            // EXPECTATION: rotation equals WEST
            robot.placeHere( PARAM_X, PARAM_Y,type::HEADING::NORTH);
            robot.rotateRight();
            auto rotation = robot.getRotation();
            ASSERT_EQUALS_INT(rotation, type::HEADING::EAST, true);

            // EXPECTATION: rotation equals SOUTH
            robot.rotateRight();
            rotation = robot.getRotation();
            ASSERT_EQUALS_INT(rotation, type::HEADING::SOUTH, true);

            // EXPECTATION: rotation equals EAST
            robot.rotateRight();
            rotation = robot.getRotation();
            ASSERT_EQUALS_INT(rotation, type::HEADING::WEST, true);

            // EXPECTATION: rotation equals NORTH
            robot.rotateRight();
            rotation = robot.getRotation();
            ASSERT_EQUALS_INT(rotation, type::HEADING::NORTH, true);
        };
        CREATE_TEST(test_toyRobot_rotate_right)

        /**
         * TEST: toyRobot output report command should return robot transform if it is within tabletop
        */
        auto test_toyRobot_output = [&](){
            object::ToyRobot& robot = m_tableTop.getPlayer();
            uint8_t PARAM_X = 0;
            uint8_t PARAM_Y = 0;
            std::string PARAM_OUTPUT = "0,0,NORTH";

            // place robot at position {0,0,NORTH}
            // EXPECTATION: output will equal PARAM_OUTPUT
            robot.placeHere( PARAM_X, PARAM_Y,type::HEADING::NORTH);
            std::string output = robot.getReport();
            ASSERT_EQUALS_STRING(output, PARAM_OUTPUT, true);

            // EXPECTATION: output will equal PARAM_OUTPUT
            PARAM_OUTPUT = "2,4,EAST";
            PARAM_X = 2;
            PARAM_Y = 4;
            robot.placeHere( PARAM_X, PARAM_Y,type::HEADING::EAST);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, PARAM_OUTPUT, true);

            // EXPECTATION: output will equal PARAM_OUTPUT
            PARAM_OUTPUT = "3,1,SOUTH";
            PARAM_X = 3;
            PARAM_Y = 1;
            robot.placeHere( PARAM_X, PARAM_Y,type::HEADING::SOUTH);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, PARAM_OUTPUT, true);

        };
        CREATE_TEST(test_toyRobot_output)

        /**
         * TEST: toyRobot input report commands should modify transform if within table top extents
        */
        auto test_toyRobot_input = [&](){

            std::string PARAM_MOVE = "MOVE";
            std::string PARAM_LEFT = "LEFT";
            std::string PARAM_RIGHT = "RIGHT";
            std::string PARAM_OUTPUT = "REPORT";

            /** EXPECTATION: Use case a
             * a)----------------
             * PLACE 0,0,NORTH
             * MOVE
             * REPORT
             * Output: 0,1,NORTH
            */
            object::ToyRobot& robot = m_tableTop.getPlayer();

            std::string PARAM_PLACE_START = "PLACE 0,0,NORTH";
            std::string PARAM_PLACE_START_POS = "0,0,NORTH";
            std::string PARAM_PLACE_FINISH_POS = "0,1,NORTH";
            robot.proccessInput(PARAM_PLACE_START);
            auto output = robot.getReport();
            robot.proccessInput(PARAM_MOVE);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, PARAM_PLACE_FINISH_POS, true);

            /** EXPECTATION: Use case a
             * a)----------------
             * PLACE 0,0,NORTH
             * LEFT
             * REPORT
             * Output: 0,0,WEST
            */
            PARAM_PLACE_START = "PLACE 0,0,NORTH";
            PARAM_PLACE_START_POS = "0,0,NORTH";
            PARAM_PLACE_FINISH_POS = "0,0,WEST";
            robot.proccessInput(PARAM_PLACE_START);
            output = robot.getReport();
            robot.proccessInput(PARAM_LEFT);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, PARAM_PLACE_FINISH_POS, true);

            /** EXPECTATION: Use case a
             * a)----------------
             * PLACE 1,2,EAST
             * MOVE
             * MOVE
             * LEFT
             * MOVE
             * REPORT
             * Output: 3,3,NORTH
            */
            PARAM_PLACE_START = "PLACE 1,2,EAST";
            PARAM_PLACE_START_POS = "1,2,EAST";
            PARAM_PLACE_FINISH_POS = "3,3,NORTH";
            robot.proccessInput(PARAM_PLACE_START);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, PARAM_PLACE_START_POS, true);
            robot.proccessInput(PARAM_MOVE);
            robot.proccessInput(PARAM_MOVE);
            robot.proccessInput(PARAM_LEFT);
            robot.proccessInput(PARAM_MOVE);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, PARAM_PLACE_FINISH_POS, true);

        };
        CREATE_TEST(test_toyRobot_input)

        /**
         * TEST: toyRobot input report commands which should all fail and not modify transform
        */
        auto test_toyRobot_invalid_input = [&](){

            std::string PARAM_MOVE = "MOVEa";
            std::string PARAM_LEFT = "LEFTa";
            std::string PARAM_RIGHT = "RIGHTa";
            std::string PARAM_OUTPUT = "REPORTa";
            std::string PARAM_PLACE_START = "PLACE 3,3,EAST";
            std::string PARAM_PLACE_INVALID_0 = "PLACE 1,1,NORTH3";
            std::string PARAM_PLACE_INVALID_1 = "PLACEs 1,1,NORTH";
            std::string PARAM_PLACE_INVALID_2 = "PLACE 1,fg,NORTH";
            std::string PARAM_PLACE_INVALID_3 = "PLACE re,1,NORTH";
            std::string PARAM_PLACE_INVALID_4 = "PLACER re,45,NORTHH";
            std::string PARAM_PLACE_INVALID_5 = "PLACE 999999,999999,NORTH";

            object::ToyRobot& robot = m_tableTop.getPlayer();
            robot.proccessInput(PARAM_PLACE_START);
            auto originalOutput = robot.getReport();

            // ACTION:      MOVE
            // EXPECTATION: The error is handled gracefully and no player state changes
            robot.proccessInput(PARAM_MOVE);
            auto output = robot.getReport();
            ASSERT_EQUALS_STRING(output, originalOutput, true);

            // ACTION:      LEFT
            // EXPECTATION: The error is handled gracefully and no player state changes
            robot.proccessInput(PARAM_LEFT);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, originalOutput, true);

            // ACTION:      RIGHT
            // EXPECTATION: The error is handled gracefully and no player state changes
            robot.proccessInput(PARAM_RIGHT);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, originalOutput, true);

            // ACTION:      OUTPUT
            // EXPECTATION: The error is handled gracefully and no player state changes
            robot.proccessInput(PARAM_OUTPUT);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, originalOutput, true);

            // ACTION:      PLACE invalid 0
            // EXPECTATION: The error is handled gracefully and no player state changes
            robot.proccessInput(PARAM_PLACE_INVALID_0);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, originalOutput, true);

            // ACTION:      PLACE invalid 1
            // EXPECTATION: The error is handled gracefully and no player state changes
            robot.proccessInput(PARAM_PLACE_INVALID_1);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, originalOutput, true);

            // ACTION:      PLACE invalid 2
            // EXPECTATION: The error is handled gracefully and no player state changes
            robot.proccessInput(PARAM_PLACE_INVALID_2);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, originalOutput, true);

            // ACTION:      PLACE invalid 3
            // EXPECTATION: The error is handled gracefully and no player state changes
            robot.proccessInput(PARAM_PLACE_INVALID_3);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, originalOutput, true);

            // ACTION:      PLACE invalid 4
            // EXPECTATION: The error is handled gracefully and no player state changes
            robot.proccessInput(PARAM_PLACE_INVALID_4);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, originalOutput, true);

            // ACTION:      PLACE invalid 5
            // EXPECTATION: The error is handled gracefully and no player state changes
            robot.proccessInput(PARAM_PLACE_INVALID_5);
            output = robot.getReport();
            ASSERT_EQUALS_STRING(output, originalOutput, true);

        };
        CREATE_TEST(test_toyRobot_invalid_input)
    };
};

#endif // UNIT_TESTS_H