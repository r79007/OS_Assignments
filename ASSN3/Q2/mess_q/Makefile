FIFO = fifo_blocking_writer fifo_blocking_reader fifo_non_blocking_writer fifo_non_blocking_reader
MSG_PASSING = message_passing_writer message_passing_reader

all: fifo msg_passing

msg_passing: $(MSG_PASSING)

fifo: $(FIFO)

fifo_blocking_writer: named_fifo_writer.c
	gcc -o fifo_blocking_writer named_fifo_writer.c

fifo_blocking_reader: named_fifo_reader.c
	gcc -o fifo_blocking_reader named_fifo_reader.c

fifo_non_blocking_writer: named_fifo_writer_nb.c
	gcc -o fifo_non_blocking_writer named_fifo_writer_nb.c

fifo_non_blocking_reader: named_fifo_reader_nb.c
	gcc -o fifo_non_blocking_reader named_fifo_reader_nb.c

message_passing_writer: message_passing_writer.c
	gcc -o message_passing_writer message_passing_writer.c

message_passing_reader: message_passing_reader.c
	gcc -o message_passing_reader message_passing_reader.c

clean:
	rm -f $(FIFO) $(MSG_PASSING)