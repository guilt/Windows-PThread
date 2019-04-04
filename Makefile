ifeq ($(OS),Windows_NT)
CC=cl
CXX=cl
CFLAGS=/nologo /MT /Ox /I Include/ /Tc
CXXFLAGS=/nologo /EHsc /MT /Ox /I Include/ /Tp
else
CC=gcc
CXX=g++
CFLAGS=-pthread
CXXFLAGS=-pthread
endif

BINS=01-test.exe 02-join.exe 03-cond.exe 04-pc.exe 05-rw.exe 06-dining.exe 07-test.exe

.phony: bin

bin: ${BINS}

%.exe: Src/%.c
	$(CC) $(CFLAGS) $< -o $@

%.exe: Src/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
