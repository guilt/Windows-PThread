Windows PThread Library
-----------------------

MinGW already ships with a decent implementation of pthreads. Visual C++
does not have one. This version of pthread.h was taken off the one provided
by Lockless Inc. and modified slightly to eliminate some errors/warnings
with C++ code.

Caveats
-------

This header is quite experimental and not as well vetted for production use.

References
----------

* Lockless Inc. - http://www.locklessinc.com/
* Various Pthread examples off the Internet
