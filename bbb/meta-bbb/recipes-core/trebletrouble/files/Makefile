bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

bin/trebletrouble: src/app.c bin/display.o
	$(CC) $(CFLAGS) $< bin/display.o -o $@

trebletrouble:	bin/trebletrouble

clean:
	rm bin/*
	rmdir bin
