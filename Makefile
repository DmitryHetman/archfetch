CC=gcc
W=-Wall -Wextra -Wpedantic -Wno-unused-result
CFLAGS=-O2 -march=native -s -fno-stack-protector -fno-plt -no-pie -pipe -o
TARGET=archfetch
static:
	$(CC) $(W) -static $(CFLAGS) $(TARGET) $(TARGET).c
	strip -s $(TARGET)
edit:
	$(EDITOR) $(TARGET).c
install:
	mv archfetch /usr/bin/
uninstall:
	rm /usr/bin/archfetch
clean:
	rm archfetch
