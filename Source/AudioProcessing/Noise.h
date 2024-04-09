#pragma once

#include <stdlib.h>
#include <time.h>

/***************************************************/
/*! \class Noise
    \brief STK noise generator.

    Generic random number generation using the
    C rand() function.  The quality of the rand()
    function varies from one OS to another.

    by Perry R. Cook and Gary P. Scavone, 1995--2014.
*/
/***************************************************/

class Noise
{
public:
    //! Default constructor that can also take a specific seed value.
    /*!
     If the seed value is zero (the default value), the random number generator is
     seeded with the system time.
     */
    Noise( unsigned int seed = 0 )
    {
        // Seed the random number generator
        this->setSeed( seed );
    }

    //! Seed the random number generator with a specific seed value.
    /*!
     If no seed is provided or the seed value is zero, the random
     number generator is seeded with the current system time.
     */
    void setSeed( unsigned int seed = 0 )
    {
        if ( seed == 0 )
            srand( (unsigned int) time( nullptr ) );
        else
            srand( seed );
    }

    //! Return the last computed output value.
    float lastOut( void ) const { return lastFrame; }

    //! Compute and return one output sample.
    float tick( void );

protected:
    float lastFrame;
};

inline float Noise :: tick( void )
{
    return lastFrame = (float) ( 2.0 * rand() / (RAND_MAX + 1.0) - 1.0 );
}
