CFLAGS=-Wall

LIBS=-lc -lasound -lsndfile -lm -lpthread -Lsrc/ugfx/.build -lugfx

INCLUDES=-Isrc/ugfx/resources/ugfx/

bin/gui.o: src/gui.c
	$(CC) -c $(CFLAGS) $< -o $@ $(INCLUDES)

bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/trebletrouble: src/ugfx/.build/libugfx.a src/app.c bin/display.o bin/input.o bin/alsa.o bin/metronome.o bin/menu.o bin/gui.o bin/tone.o bin/libfft.o bin/fileRead.o bin/fileFormat.o bin/find_freq.o bin/filter.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(INCLUDES)

src/ugfx/.build/libugfx.a:
	$(MAKE) -C src/ugfx

trebletrouble:	bin/trebletrouble

clean:
	./scripts/clean.sh
