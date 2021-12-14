#ifndef OBJECTS_H
#define OBJECTS_H

/**
 * @brief 
 * 
*/

#include "Types.h"
#include <memory>
#include <functional>

/// table top sizes
#define TABLE_TOP_X 4
#define TABLE_TOP_Y 4

namespace object
{

    /**
     * Input processing base class
     * @brief manages logic around input and input validation  
    */
    class InputHandler
    {
    protected:
        /// key/value pairs linkning action enums to lambdas
        std::map<type::ACTION, std::function<void(const std::string _input)>> m_actionMap;

        /**
         * @brief check a value against X axie rules
         * @param _x the value to validate
         * @return pass or fail
        */
        const bool validateAxisX(const uint32_t& _x);

        /**
         * @brief check a value against Y axie rules
         * @param _y the value to validate
         * @return pass or fail
        */
        const bool validateAxisY(const uint32_t& _y);

        /**
         * @brief check a value against rotation rules
         * @param _heading the value to validate
         * @return pass or fail
        */
        const bool validateRotation(const type::HEADING& _heading);

        /**
         * @brief check if a string is a number
         * @param string to validate
         * @return pass or fail
        */
        const bool isNumber(const std::string& _str);
    public:

        InputHandler(){};
        virtual ~InputHandler(){};

        /**
         * @brief process strinified commands
         * @param string command to process
        */
       void proccessInput(const std::string& _input)
       {
            // get command out of input string
            const std::string actionStr = _input.substr(0, _input.find(" "));
            // get str to enum value iterator
            auto enumItr = type::actionEnumMap.find(actionStr);
            // does input exist as an action enum, if not return early
            if(enumItr == type::actionEnumMap.cend())
                return;
            // does action enum have a associated action callback, if not return early
            const auto actionItr = m_actionMap.find(enumItr->second);
            if(actionItr == m_actionMap.cend())
                return;
            // run action callback
            actionItr->second(_input);
       };
    private:

        /**
         * @brief pure virtual method to build action and lambda map 
        */
        virtual void _buildActions() = 0;
    };

    /**
     * Player controller robot
    */
    class ToyRobot : public InputHandler
    {
        /// robot position and rotation
        type::T_Transform<uint8_t> m_transform;
        /// flag identying if this robot has been placed
        bool m_hasBeenPlaced = false;

    public:
        ToyRobot();
        ToyRobot(const ToyRobot&) = delete;
        void operator=(const ToyRobot&) = delete;
    private:

        /**
         * @brief logic to rotate robot transform
         * @param bool flag to indicate direction to rotate
        */
        void _rotate(bool _clockWise);

        /**
         * @brief builds action and lambda map for robot
        */
        virtual void _buildActions() override;

    public:

        /**
         * @brief moves robot 1 unit in the direct it is facing
        */
        void move();

        /**
         * @brief rotates robot to the left/anti-clockwise direction
        */
        void rotateLeft();

        /**
         * @brief rotates robot to the right/clockwise direction
        */
        void rotateRight();

        /**
         * @brief places the robot at a specific location on the table top
         * @param uint8_t x axis on table top
         * @param uint8_t y axis on table top
         * @param HEADING direction to face
        */
        void placeHere(const uint8_t _x, const uint8_t _y, const type::HEADING _rotation);

        /**
         * @brief get the tabl top position of the robot
         * @return T_Position<uint8_t> position data struct
        */
        const type::T_Position<uint8_t> getPosition();

        /**
         * @brief get the global rotation
         * @return HEADING global direction of robot  
        */
        const type::HEADING getRotation();

        /**
         * @brief ouput position data to the window
         * @return string form of the global position
        */
        const std::string getReport();
    };

    /**
     * table top world
    */
    struct TableTop
    {
    private:
        /// extents of map
        const unsigned m_axisX;
        const unsigned m_axisY;
        /// ownership of player toy robot
        std::unique_ptr<object::ToyRobot> m_toyRobot;

    public:
        TableTop(const unsigned _x, const unsigned _y)
            :   m_axisX(_x), 
                m_axisY(_y), 
                m_toyRobot(std::make_unique<object::ToyRobot>())
        {};

        /**
         * @brief accessors for X and Y boundries
         * @return unsigned current boundry values
        */
        const unsigned & getExtentX() const { return m_axisX; };
        const unsigned & getExtentY() const { return m_axisY; };

        /**
         * @brief accessors for player ToyRobot
         * @return ToyRobot refference to unique toy robot
        */
        object::ToyRobot& getPlayer() const
        {
            return *m_toyRobot.get();
        };
    };
}

#endif  // OBJECTS_H
