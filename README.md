# C++ Template Libraries #

*   CNTL : C++ Non-Standard Template Library

Wheels reinvented.

    Last Update: Jun 29, 2012
    Since: Jan 01, 2011

## Contents ##

*   Containers

  *   DynamicArray : Reinvented std::vector with some modifications.
  *   AssocArray : Ordered assosiative map (std::map) implemented with cntl::DynamicArray.
  *   SortedArray : Ordered array (std::set) implemented with cntl::DynamicArray.
  *   LinkedList : Doubly-linked list implementation.
  *   String : C string wrapper.
  *   UnFunctor : 1-argument functor.

*   Utilities

  *   WELL : WELL1024a random number generator.

*   Memory Management

  *   Memory Allocator (Allocate, AllocateTail, AllocateAligned, Free, Reallocate)
  *   STL Allocator


## Building how-to ##

    $ cd build
    $ cmake ..
    $ start cntl.sln


## Credits ##

*   vaiorabbit (http://twitter.com/vaiorabbit)

 
## License ##

All source codes are available under the terms of the zlib/libpng license
(see doc/LICENSE.txt).
