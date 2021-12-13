#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <memory>
#include <map>
#include <climits>
#include <limits>
#include <string>

namespace type
{
    /**
     * Actions that a user can take
    */
    enum class ACTION : uint8_t
    {
        MOVE    = 0b000, /// 0
        LEFT    = 0b001, /// 1
        RIGHT   = 0b010, /// 2
        PLACE   = 0b011, /// 3
        REPORT  = 0b100  /// 4
    };
    const std::map<std::string, ACTION> actionEnumMap =
    {
        { "MOVE", ACTION::MOVE },
        { "LEFT", ACTION::LEFT },
        { "RIGHT", ACTION::RIGHT },
        { "PLACE", ACTION::PLACE },
        { "REPORT", ACTION::REPORT }
    };

    /**
     * Avaliable headings on the table top
    */
    enum HEADING
    {
        NORTH       = 0b00,
        EAST        = 0b01,
        SOUTH       = 0b10,
        WEST        = 0b11,
        UNDEFINED   = 0b111
    };
    const std::map<HEADING, std::string> headingMap =
    {
        { HEADING::NORTH,       "NORTH"     },
        { HEADING::EAST,        "EAST"      },
        { HEADING::SOUTH,       "SOUTH"     },
        { HEADING::WEST,        "WEST"      },
        { HEADING::UNDEFINED,   "UNDEFINED" }
    };
    /**
     * @brief convert string to enum
     * @param _heading string to convert
     * @return HEADING converion for string
    */
    inline HEADING getHeadingEnum(const std::string& _heading)
    {
        const std::map<std::string, HEADING> enumToStr =
        {
            { "NORTH", HEADING::NORTH },
            { "EAST", HEADING::EAST,    },
            { "SOUTH", HEADING::SOUTH },
            { "WEST", HEADING::WEST  },
            { "UNDEFINED", HEADING::UNDEFINED  }
        };
        // get enum if it exists
        auto itr = enumToStr.find(_heading);
        if(itr != enumToStr.cend())
            return itr->second;
        // return undefined if querry fails
        return HEADING::UNDEFINED;
    }

    /**
     * make bit size check precompile
     * this allows it to be used in calculating enum value
    */
    template <typename T>
    constexpr std::size_t
    bit_size() noexcept
    {
        return sizeof(T) * CHAR_BIT;
    }

    /**
     * Data structure for position
     * @brief   this struct has been templated to future proof table top extents size changes.
     *          currently only a 5x5 table is needed so a uint8_t is big enough to hold both x and y.
     *          x and y are in bit fields to conserve memory space
    */
    template <typename T> 
    struct T_Position {
        T x : (type::bit_size<T>()/2)-1;
        T y : (type::bit_size<T>()/2)-1;
        T_Position(T _x = 0, T _y = 0 )
            : x(_x),
            y(_y)
        {};
    };

    /**
    *   Builds tramsform for for following types
    *   @brief  this struct has been templated to future proof table top extents size changes.
     *          currently only a 5x5 table is needed so a uint8_t is big enough to hold both position and rotation.
    *           position x, y and rotation use a single variable
    *           uint8_t: 11000000 signifies the heading flags
    *           uint8_t: 00111000 signifies the position y
    *           uint8_t: 00000111 signifies the position x
    *   @param uint8_t  8 bits total  -> 3 bits for each position axis  -> 0-7
    *   @param uint16_t 16 bits total -> 7 bits for each position axis  -> 0-127
    *   @param uint32_t 32 bits total -> 15 bits for each position axis -> 0-32767
    */
    template <typename T> 
    struct T_Transform 
    { 
    private:
        /// transform single data variable
        T data = ~(T() & 0);

        /**
         * Bitwise mask depending on T size
        */
        enum MASK : T
        {
            // casts unsigned type so signed bits are included
            // recasting is required as after bitwise operation compilers assume signed char
            // Set all bits to 1 for any unsigned type: T(~(T() & 0)))
            ROTATION = T(T(~(T() & 0)) << (type::bit_size<T>()-2)),
            AXIS_X   = T(T(~(T() & 0)) >> ((type::bit_size<T>()/2)+1)),
            AXIS_Y   = T(T(~(T() & 0)) ^ (AXIS_X | ROTATION))
        };

    public:

        /**
         * @brief modify position 
         * @param T_Position<T> new position to set
        */
        void setPosition(const T_Position<T> pos)
        {
            // clear X and Y axis bits and leave rotation bits
            data &= MASK::ROTATION;
            // set X axis by isolation
            data |= (MASK::AXIS_X & pos.x);
            // set Y axis by isolation and shift to the left
            data |= ((MASK::AXIS_X & pos.y) << (type::bit_size<T>()/2)-1);
        }

        /**
         * @brief access position 
         * @return T_Position<T> position
        */
        const T_Position<T> getPosition()
        {
            // return Position struct
            // isolate X and Y data with masks
            return T_Position<T>(
                (data & MASK::AXIS_X),
                ((data & MASK::AXIS_Y) >> ((type::bit_size<T>()/2)-1))
            );
        };

        /**
         * @brief access rotation 
         * @return HEADING rotation
        */
        type::HEADING getRotation()
        {
            // isolate roation by combining a clear with X and Y mask and shifting to the right
            return type::HEADING((data & ~(MASK::AXIS_Y | MASK::AXIS_X)) >> (type::bit_size<T>()-2));
        }

        /**
         * @brief modify rotation 
         * @param HEADING new rotation to be set
        */
        void setRotation( type::HEADING _heading)
        {
            // clear rotation bits
            data &= (MASK::AXIS_Y | MASK::AXIS_X);
            // set rotation by shifting _heading to the last 2 spots 
            data |= (_heading << (type::bit_size<T>()-2));
        }
        
    }; 
}

#endif  // TYPES_H