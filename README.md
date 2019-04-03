Windows PThread Library
-----------------------

MinGW already ships with a decent implementation of pthreads. Visual C++
does not have one. This version of pthread.h was taken off the one provided
by Lockless Inc., and modified slightly to eliminate some errors/warnings.

Caveats
-------

When testing with Visual C++, the header was found to work in C but
not in C++ code. Beware!

References
----------

http://www.locklessinc.com/
