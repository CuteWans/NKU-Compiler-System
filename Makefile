.PHONY: myyacc, in2post, clean

myyacc:
	bison myyacc.y
	gcc myyacc.tab.c -o myyacc

in2post:
	bison in2post.y
	gcc in2post.tab.c -o in2post

clean:
	rm -rf *.tab.c in2post myyacc