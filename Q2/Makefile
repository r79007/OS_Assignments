# _*_Makefile_*_
#target: dependencies
#	action


all: P1_fifo P2_fifo P1_mesq P2_mesq P1_sockets P2_sockets

P1_fifo: P1_fifo.c
	gcc P1_fifo.c -o P1_fifo

P2_fifo: P2_fifo.c
	gcc P2_fifo.c -o P2_fifo

P1_mesq: P1_mesq.c
	gcc P1_mesq.c -o P1_mesq

P2_mesq: P2_mesq.c
	gcc P2_mesq.c -o P2_mesq

P1_sockets: P1_sockets.c
	gcc P1_sockets.c -o P1_sockets

P2_sockets: P2_sockets.c
	gcc P2_sockets.c -o P2_sockets

clean:
	rm -f P1_fifo P2_fifo P1_mesq P2_mesq P1_sockets P2_sockets fifo1 fifo2 Socket