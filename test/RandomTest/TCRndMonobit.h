// -*- mode: C++; coding: utf-8 -*-
#ifndef TCRNDMONOBIT_H_INCLUDED
#define TCRNDMONOBIT_H_INCLUDED

#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cntl/Random.h>
#include <TestFramework.h>

class TCRndMonobit : public Test::Case
{
    const float tolerance;

public:
    TCRndMonobit( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    /// <summary>
    /// The monobit test
    /// Ref.:
    ///   FIPS 140-2
    ///   http://csrc.nist.gov/publications/fips/fips140-2/fips1402.pdf
    /// </summary>
    /// <param name="seed">seed value for the WELL constructor.</param>
    /// <returns>true if the random sequence passes the test.</returns>
    static bool MonobitTest( unsigned int seed )
        {
            unsigned int initial_state[cntl::WELL::StateDimension];
            unsigned int key = seed;
            for ( int i = 0; i < cntl::WELL::StateDimension; ++i )
            {
                // Thomas Wang's 32 bit Mix Function
                // http://www.concentric.net/~Ttwang/tech/inthash.htm
                key = ~key + (key << 15);
                key =  key ^ (key >> 12);
                key =  key + (key << 2);
                key =  key ^ (key >> 4);
                key =  key * 2057;
                key =  key ^ (key >> 16);
                initial_state[i] = key;
            }

            cntl::WELL rng( initial_state );
            unsigned long cnt = 0;

            for ( int i = 0; i < (20000 / 32); ++i )
            {
                unsigned int r = rng.Next();
                while ( r != 0 )
                {
                    if ( (r & 1) == 1 )
                        ++cnt;
                    r >>= 1;
                }
            }

            return (9725 < cnt && cnt < 10275) ? true : false;
        }

    virtual void Run()
        {
            const int tests = 10000;
            int random_seed = 0;
            int passed = 0;

            for (int i = 0; i < tests; ++i)
            {
                if ( MonobitTest((unsigned int)rand()) )
                    ++passed;
            }

            TEST_ASSERT( passed > 9990 );

            std::stringstream ss;
            ss << "Passed/Total = " << passed << "/" << tests;
            std::cout << ss.str() << std::endl;
        }
};

#endif
