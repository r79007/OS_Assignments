To compile the programs type make and press enter.

For question 2, I created 3 pairs of programs, 2 for each method(fifo, message queue, sockets). The P1 for each method has the format P1_<method_name>(for ex for fifo P1_fifo, for message queue P1_mesq and for sockets, P1_sockets), similarily P2 of each method has the format P2_<method_name>. Run P1 and then P2.

FIFO

FIrst I generated 50 random strings of length 4 using the function randomString() and stored it in a char array called buffer2.

In P1_fifo.c, first I made a fifo called fifo1 using mkfifo(“fifo1”,0666), then I opened these fifos using open() syscall(write only for fifo1 and read only for fifo2), also I opened fifo2(another fifo) (which was made using mkfifo(“fifo2”,0666) in P1_fifo.c(this fifo is for P2_fifo.c to send the highest ID received to P1_fifo.c

Then in an infinite while loop, I made a while loop that terminates when 50 strings are sent and then(in a for loop that runs for 5 times) I stored the strings one by one along with their IDs(with a space) in an character array called data2, and then in each each iteration I used write() syscall to write the content of data2 to fifo1, also i read the highest ID received everytime using read() syscall to read the contents of fifo2, and then I stored the ID into a variable called strs and then started sending 5 strings from this index, and this process will continue until strs becomes =50.

In P2_fifo.c, I opened both the fifos(fifo1(read-only) and  fifo2(write-only)), and I used an infinite while loop in which I first read what was written fifo1 and then displayed the string and its id using printf then I sent the highest ID received everytime to P1_fifo.c using fifo2(by using write() syscall) and then broke from the while loop when the number of strings received became 50.

And then in both programs I closed the file descriptors corresponding to fifo1 and fifo2.

Sockets

FIrst I generated 50 random strings of length 4 using the function randomString() and stored it in a char array called buffer2.(In P1_sockets.c)

In P1_sockets.c, then I used unlink() and passed the name of the socket to delete any existing files with the same name as out socket name and then I created a socket with socket name “Socket” using socket() function and passed in the appropriate arguments in it and then stored the file descriptor of the socket in the variable named connection_socket.
Then I populated the struct sockaddr_un name; Now to bind that socket file descriptor to a unix domain I used bind() function and to prepare the socket for incoming connections I used listen(), then after this in an infinite loop I used accept to accept connection from P2_sockets.c, and stored the file descriptor returned by accept() in a variable called data_socket, then I proceeded as I did in fifo’s P1. I wrote the string with a space and its ID to this file descriptor and then read the highest IDs received from P2, and started from the highest ID received after sending 5 strings, until the number of strings sent become 50 then I broke off from the infinite for loop and closed the data_socket file descriptor.

In P2_sockets.c I did the same as I did in fifo’s P2. First of all to connect to the socket made by P1_sockets.c I did the same steps till connect() and then I did the same as fifo’s P2(but I only used 1 file descriptor unlike fifo). Inside an infinite while loop I kept on receiving the data and then printed the strings received and their IDs and then sent the ids of the received strings to P1_sockets, and then broke off the infinite while loop when the number of strings received became 50. Then I closed the socket by using close().

Message Queue

FIrst I generated 50 random strings of length 4 using the function randomString() and stored it in a char array called buffer.(In P1_mesq.c).

In P1_mesq.c to make the message queue, first I generated the key for the message queue by using ftok(), then I used msgget() to get the message queue id. Also I made two structs message1 and message2, message 1 is used to send the messages(using msgsnd) of type 1(message.type=1) that are the strings along with the IDs that P1_mesq.c is sending to P2_mesq.c(in batches of 5), then after these 5 strings are sent I use sleep(1) to wait for the P2_mesq to send the message2 struct(that contains the highest id received) to P1_mesq, and then started off from the highest id received(using msgrcv for message type 2) and continued this untill the highest id recieved becomes 49.

In P2_mesq.c I followed the same steps as in P1_mesq.c till msgget. But in the infinite for loop I first used msgctl() with the second parameter as IPC_STAT and third parameter as &buf, so as to populate the struct msqid_ds buf, so that i can use the bug.msg_qnum to get the number of messages in the queue and then inside a do while loop I continue to receive the messages using msgrcv() and then I printed them, and then i again populate the struct msqid_ds buf using msgctl() and i continue this process till there are no messages remaining in the queue. Then I send the highest id received using the message2 struct and speify the message2.type=2 and send it using msgsnd() to P1_mesq.c. This entire process continues till the number of strings received is not equal to 50, after which we break off the infinite for loop.
Then I finally destroy the message queue using msgctl() by passing the second argument as IPC_RMID.

The message1 and message2 structs contain 3 entities, type, a char array of type arr[50][8](to store 50 strings of size 8 bytes max) and a char array for max id received of size 10 bytes called maxId.

In all of the above methods while making a fifo, message queue and socket I have specified 0666 for read and write permissions to owner, group members, other users.

