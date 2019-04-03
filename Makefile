CC=cl
CFLAGS=/nologo /MT /Ox /I Include/ /Tc
BINS=01-test.exe 02-join.exe 03-cond.exe 04-pc.exe 05-rw.exe 06-dining.exe

.phony: bin

bin: ${BINS}

%.exe: Src/%.c
	$(CC) $(CFLAGS) $<
