// -*- mode: C++; coding: utf-8 -*-
#ifndef TCAAPERFORMANCE_H_INCLUDED
#define TCAAPERFORMANCE_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <map>
#include <cntl/AssocArray.h>
#include <cntl/DynamicArray.h>
#include <cntl/Random.h>
#include <cntl/Timer.h>
#include <TestFramework.h>
#include "Vector3.h"

#define N_ARRAY_SIZE 10000

class TCAAPerformance : public Test::Case
{
    const float tolerance;
    cntl::WELL rng;

public:
    TCAAPerformance( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        , rng()
        {}

    virtual void Initialize()
        {
            rng.Reset( 37 );
        }

    virtual void Run()
        {
            cntl::DynamicArray< std::pair<unsigned int, unsigned int> > narray;
            for ( int i = 0; i < N_ARRAY_SIZE; ++i )
            {
                narray.push_back( std::make_pair(rng.Next(), rng.Next()) );
            }

            cntl::Timer timer;

            // std::map
            std::cout << "\n=== std::map ===\n";
            {
                typedef std::map<int, int, std::less<int>, cntl::STLAllocator<std::pair<int,int> > > IntMap;
                // typedef std::map<int, int, std::less<int>, std::allocator<std::pair<int,int> > > IntMap;
                IntMap m;

                timer.Reset();
                for ( int i = 0; i < N_ARRAY_SIZE; ++i )
                {
                    m[rng.Next()] = rng.Next();
                }
                std::cout << "insert (std::map): " << timer.GetElapsedTime() << "sec" << std::endl;

                timer.Reset();
                for ( int i = 0; i < N_ARRAY_SIZE; ++i )
                {
                    m.find(rng.Next());
                }
                std::cout << "find (std::map): " << timer.GetElapsedTime() << "sec" << std::endl;

                // cntl::AllocatorForSTL().Dump();
                std::cout << "Free size sum (std::map): " << cntl::AllocatorForSTL().GetFreeSizeSum() << std::endl;
            }

            // cntl::AssocArray
            std::cout << "\n=== cntl::AssocArray ===\n";
            {
                typedef cntl::AssocArray<int, int, std::less<int>, cntl::STLAllocator<std::pair<int,int> > > IntMap;
                // typedef cntl::AssocArray<int, int, std::less<int>, std::allocator<std::pair<int,int> > > IntMap;
                IntMap m;

                timer.Reset();
                for ( int i = 0; i < N_ARRAY_SIZE; ++i )
                {
                    m[rng.Next()] = rng.Next();
                }
                std::cout << "insert (cntl::AssocArray): " << timer.GetElapsedTime() << "sec" << std::endl;

                timer.Reset();
                for ( int i = 0; i < N_ARRAY_SIZE; ++i )
                {
                    m.find(rng.Next());
                }
                std::cout << "find (cntl::AssocArray): " << timer.GetElapsedTime() << "sec" << std::endl;

                // cntl::AllocatorForSTL().Dump();
                std::cout << "Free size sum (std::map): " << cntl::AllocatorForSTL().GetFreeSizeSum() << std::endl;
            }
        }
};

#endif
