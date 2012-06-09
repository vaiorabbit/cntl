// Ref.: C++ Cookbook http://www.ryanstephens.com/cookbook.html
#include <iostream>
#include <cntl/Memory.h>
#include "TokenizerSample.h"

int main( int argc, char** argv )
{
    cntl::MemoryAllocatorProperty property;
    property.m_uAreaSizeForSystemHeap = 1 * 1024 * 1024;
    property.m_uAreaSizeForSTLHeap = 1 * 1024 * 1024;
    cntl::MemoryAllocatorInitialize( property );

    {
        cntl::String s = " razzle dazzle giddyup ";
        cntl::String tmp;

        StringTokenizer st( s );

        std::cout << "there are " << st.countTokens() << " tokens.\n";
        while ( st.hasMoreTokens() )
        {
            st.nextToken( tmp );
            std::cout << "token = " << tmp << '\n';
        }
    }

    cntl::MemoryAllocatorFinalize();

    return 0;
}
