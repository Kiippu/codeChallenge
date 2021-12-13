#include "Objects.h"
#include <string>
#include <cstring>

namespace object
{
    ToyRobot::ToyRobot()
    {
        // build actions for each input enum
        _buildActions();
    };

    void ToyRobot::_rotate(bool _clockWise)
    {
        // check if the robot is on the table top
        if(!m_hasBeenPlaced)
            return;
        // get transform heading
        const type::HEADING direction = m_transform.getRotation();
        // get heading from map
        auto itr = type::headingMap.find(direction);
        // sanity check that the heading enum exists in map
        if(itr != type::headingMap.cend())
        {
            // rotate clockwise
            if(_clockWise)
            {
                // increment iterator in clockswise direction
                ++itr;
                // is the itr now past the end of the map
                // if so then make the iter equal first iteration
                if(itr->first == type::HEADING::UNDEFINED)
                    itr = type::headingMap.begin();
            }
            // rotate in anti-clockwaise direction
            else
            {
                // pre check if itr is first in map 
                if(itr == type::headingMap.cbegin())
                {
                    // itr pointed at first element and is unsafe to decrement
                    // set itr to end() and decrement from there
                    itr = type::headingMap.end();
                    --itr;
                }
                else
                {
                    // itr is safe to decement
                    --itr;
                }
            }
            // apply new rotation
            m_transform.setRotation(itr->first);
        }
    }

    void ToyRobot::move()
    {
        // check if the robot is on the table top
        if(!m_hasBeenPlaced)
            return;
        auto currentPos = getPosition();
        // depending on heading move in that direction
        switch (getRotation())
        {
        // move to the north
        case type::HEADING::NORTH:
            if(validateAxisX(++currentPos.y))
            {
                m_transform.setPosition(currentPos);
            }
            break;
        // move to the south
        case type::HEADING::SOUTH:
            if(validateAxisX(--currentPos.y))
            {
                m_transform.setPosition(currentPos);
            }
            break;
        // move to the east
        case type::HEADING::EAST:
            if(validateAxisX(++currentPos.x))
            {
                m_transform.setPosition(currentPos);
            }
            break;
        // move to the west
        case type::HEADING::WEST:
            if(validateAxisX(--currentPos.x))
            {
                m_transform.setPosition(currentPos);
            }
            break;
        // anything else do nothing
        default:
            break;
        }
    }

    void ToyRobot::rotateLeft()
    { 
        _rotate(false); 
    }
    
    void ToyRobot::rotateRight()  
    { 
        _rotate(true); 
    }
    
    void ToyRobot::placeHere(const uint8_t _x, const uint8_t _y, const type::HEADING _rotation)
    {
        // check validity of arguments
        if(validateAxisX(_x) && validateAxisY(_y) && validateRotation(_rotation))
        {   
            // set a new position and rotation
            m_hasBeenPlaced = true;
            m_transform.setRotation(_rotation);
            const type::T_Position<uint8_t> position(_x, _y);
            m_transform.setPosition(position);
        }
    }
    
    const type::T_Position<uint8_t> ToyRobot::getPosition()
    {
        return m_transform.getPosition();
    }
    
    const type::HEADING ToyRobot::getRotation()
    {
        return m_transform.getRotation();
    }

    const std::string ToyRobot::getReport()
    {
        // build a sting for the position and rotation
        std::string result;
        auto position = m_transform.getPosition();
        uint x = position.x;
        uint y = position.x;
        auto rotationStr = type::headingMap.at(getRotation());
        result.append(std::to_string(position.x)).append(",");
        result.append(std::to_string(position.y)).append(",");
        result.append(rotationStr);
        return result;
    }

    void ToyRobot::_buildActions()
    {
        // add PLACE action 
        m_actionMap.insert(std::make_pair( type::ACTION::PLACE , [&](const std::string& _input)
        {
            // get modifiable string
            std::string input(_input);
            // remove place command
            input.erase(0, 6);
            // split string with ","
            std::string delimiter = ",";
            size_t pos = 0;
            uint32_t loopCounter = 0;
            uint32_t x = 0, y = 0;
            type::HEADING heading; 
            std::string token;
            bool tooManyTokens = false;
            // iterate over input to build command from string
            while (input.size() > 0) 
            {
                // the the new delimiter position
                pos = input.find(delimiter);
                token = input.substr(0, pos);
                // extract data depending on loop count
                switch (loopCounter)
                {
                // extract X value if its a number
                case 0:
                    if(!isNumber(token)) return;
                    x = atoi(token.c_str());
                break;
                // extract Y value if its a number
                case 1:
                    if(!isNumber(token)) return;
                    y = atoi(token.c_str());
                break;
                // extract heading and clear token str
                case 2:
                    heading = type::getHeadingEnum(token);
                    input.clear();
                break;
                // the input was too long and therefore the whole input is classified as a fail
                default:
                    pos = std::string::npos;
                    tooManyTokens = true;
                break;
                }
                // remove token from input
                input.erase(0, pos + delimiter.length());
                // increment loop counter
                ++loopCounter;
            }
            // check for and fail criteria
            bool hasPassed = validateAxisX(x) && validateAxisY(y) && validateRotation(heading);
            if(hasPassed && !tooManyTokens)
                placeHere( x, y, heading);
        }));
        // add MOVE action 
        m_actionMap.insert(std::make_pair( type::ACTION::MOVE , [&](const std::string&){
            // check if the robot is on the table top
            if(!m_hasBeenPlaced)
                return;
            move();
        }));
        // Left action 
        m_actionMap.insert(std::make_pair( type::ACTION::LEFT , [&](const std::string&){
            // check if the robot is on the table top
            if(!m_hasBeenPlaced)
                return;
            rotateLeft();
        }));
        // Right action 
        m_actionMap.insert(std::make_pair( type::ACTION::RIGHT , [&](const std::string&){
            // check if the robot is on the table top
            if(!m_hasBeenPlaced)
                return;
            rotateRight();
        }));
        // Report action 
        m_actionMap.insert(std::make_pair( type::ACTION::REPORT , [&](const std::string&){
            // check if the robot is on the table top
            if(!m_hasBeenPlaced)
                return;
            printf( "Output : %s\n" ,getReport().c_str());
        }));
    };

}

namespace object
{

    const bool InputHandler::isNumber(const std::string& _str)
    {
        for(const auto& c : _str)
            if (std::isdigit(c) == 0) return false;
        return true;
    }

    const bool InputHandler::validateAxisX(const uint32_t& _x)
    {
        // check value against extents
        return (_x <= TABLE_TOP_X) && (_x >= 0);
    }

    const bool InputHandler::validateAxisY(const uint32_t& _y)
    {
        // check value against extents
        return (_y <= TABLE_TOP_X) && (_y >= 0);
    }

    const bool InputHandler::validateRotation(const type::HEADING& _heading)
    {
        // check value is defined
        return (_heading != type::HEADING::UNDEFINED);
    }
}
