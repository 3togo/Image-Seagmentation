#qiaozh_lin
#2015-11-19 45:28

CC=g++
CFLAGS=-g -w `pkg-config --cflags  opencv4`
CFLAGS1=`pkg-config --libs opencv4`

all: img_seg

img_seg: main.cpp segment.cpp datastrcture.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(CFLAGS1)

clean:
	rm img_seg
