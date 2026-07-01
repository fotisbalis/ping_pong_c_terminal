all:
	gcc ping_pong.c -std=c11 -o ping_pong -lncurses

clean:
	rm -f ping_pong
