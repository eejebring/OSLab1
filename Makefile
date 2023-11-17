
ADD_US_SRC = "timespec_add_usec.c"


ragnar:
	gcc ragnar.c $(ADD_US_SRC) -o ragnar.bin

hello:
	gcc hello_thread.c $(ADD_US_SRC) -o ht.bin

clean:
	-rm *.bin

L1E1:
	-gcc L1E1.c -lpthread -lwiringPi -llttng-ust -o L1E1.bin
