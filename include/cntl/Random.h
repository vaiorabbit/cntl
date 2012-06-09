// -*- mode: C++; coding: utf-8 -*-
#ifndef CNTL_RANDOM_H_INCLUDED
#define CNTL_RANDOM_H_INCLUDED

//
// * Features 
//
//   * class WELL : WELL1024a random number generator
//
//     Ref.:
//       L'Ecuyer, Pierre; Panneton, François; Matsumoto, Makoto (2006), 
//       Improved Long-Period Generators Based on Linear Recurrences Modulo 2
//       http://www.iro.umontreal.ca/~lecuyer/myftp/papers/wellrng.pdf
//       http://www.iro.umontreal.ca/~lecuyer/myftp/papers/wellrng-errata.txt
//       http://www.iro.umontreal.ca/~panneton/WELLRNG.html
//
//       Chris Lomont (2007), Random Number Generation
//       http://www.lomont.org/Math/Papers/2008/Lomont_PRNG_2008.pdf
//

namespace cntl
{
    class WELL
    {
        class State
        {
        public:

            static const int Dimension = 32;

        private:

            unsigned int state_index;
            unsigned int state[Dimension];

        public:

            static const unsigned int m1 = 3;
            static const unsigned int m2 = 24;
            static const unsigned int m3 = 10;

            static const unsigned int r_minus_1 = 31;
            static const unsigned int new_v1 = 0;
            static const unsigned int new_v0 = 31;

            State()
            {}

            /*
             * Using the typical get set properties in C#... with parameters - Stack Overflow
             * http://stackoverflow.com/questions/236530/using-the-typical-get-set-properties-in-c-with-parameters
             */
            unsigned int& operator[]( unsigned int i )
            {
                // get { return state[(state_index + i) & 0x0000001f]; }
                // set { state[(state_index + i) & 0x0000001f] = value; }
                return state[(state_index + i) & 0x0000001f];
            }

            void Advance()
            {
                state_index = (state_index + 31) & 0x0000001f;
            }

            void Reset( unsigned int initial_state[Dimension] )
            {
                state_index = 0;
                for ( int i = 0; i < Dimension; ++i )
                    state[i] = initial_state[i];
            }
        }; // class State


        State v;

    public:

        static const int StateDimension = State::Dimension;

        WELL( unsigned int initial_state[State::Dimension] )
            : v()
        {
            Reset( initial_state );
        }

        void Reset( unsigned int initial_state[State::Dimension] )
        {
            v.Reset( initial_state );
        }

        unsigned int Next()
        {
            unsigned int z0 = v[State::r_minus_1];
            unsigned int z1 = v[0] ^ (v[State::m1] ^ (v[State::m1] >> 8));
            unsigned int z2 = (v[State::m2] ^ (v[State::m2] << 19)) ^ (v[State::m3] ^ (v[State::m3] << 14));
            unsigned int z3 = z1 ^ z2;
            unsigned int z4 = (z0 ^ (z0 << 11)) ^ (z1 ^ (z1 << 7)) ^ (z2 ^ (z2 << 13));
            v[State::new_v1] = z3;
            v[State::new_v0] = z4;

            v.Advance();

            return v[0];
        }

    }; // class WELL
}

#endif
