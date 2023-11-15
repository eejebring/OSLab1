
ADD_US_SRC = "timespec_add_usec.c"


#ragnar:
	#gcc ragnar.c $(ADD_US_SRC) -o ragnar.bin

hello:
	gcc hello_thread.c $(ADD_US_SRC) -o ht.bin

clean:
	-rm *.bin

L1C1:
	-gcc L1C1.c -lpthread -lwiringPi -o L1C1.bin
