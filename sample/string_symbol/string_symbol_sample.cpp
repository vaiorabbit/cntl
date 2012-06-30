#include <iostream>
#include <map>

#include <cntl/Memory.h>
#include "string_symbol.h"

#if defined(__APPLE__) || defined(__linux__)
// Ref.: http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm
# include <sys/timeb.h>

int GetTickCount()
{
    timeb tb;
    ftime( &tb );
    int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
    return nCount;
}
#elif defined(WIN32)
# include <windows.h>
#endif

// Ref.: stx::basic_symbol<T> http://www.s34.co.jp/cpptechdoc/misc/stx-basic_symbol/

const int N = 1000;

cntl::String       string_table[N];
stx::string_symbol symbol_table[N];

void init() {
  char buf[32];
  cntl::String key;
  for ( int i = 0; i < N; ++i ) {
    sprintf(buf,"string as comparison key:%d",i);
    string_table[i] = buf;
    symbol_table[i] = stx::string_symbol(buf);
  }
}

template<class Container, class T>
long trial(Container& container, const T* table, int repeat) {
    int i;
    // table[0..N-1]をcontainerに挿入する。
    for (i = 0; i < N; ++i)
        container[table[i]] = i;

    // table[0..N-1]をcontainerから検索し、その処理時間を求める。
    long t = GetTickCount();
    while ( repeat-- )
        for ( i = 0; i < N; ++i)
            container.find(table[i]);
    return GetTickCount() - t;
}

int main( int argc, char** argv )
{
    cntl::MemoryAllocatorProperty property;
    property.m_uAreaSizeForSystemHeap = 1 * 1024 * 1024;
    property.m_uAreaSizeForSTLHeap = 1 * 1024 * 1024;
    cntl::MemoryAllocatorInitialize( property );

    {
        std::map<cntl::String,int>       strmap;
        std::map<stx::string_symbol,int> symmap;

        init();

        std::cout << "strmap " << trial(strmap, string_table, 100) << " [ms]\n";
        std::cout << "symmap " << trial(symmap, symbol_table, 100) << " [ms]\n";
    }

    cntl::MemoryAllocatorFinalize();

    return 0;
}
