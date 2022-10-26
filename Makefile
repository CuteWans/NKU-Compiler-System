.PHONY: all clean

all: myyacc in2post

myyacc: myyacc.y
	bison myyacc.y
	gcc myyacc.tab.c -o myyacc

in2post: in2post.y
	bison in2post.y
	gcc in2post.tab.c -o in2post

clean:
	rm -rf *.tab.c in2post myyacc