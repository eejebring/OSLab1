# Exercise 1
question:
systemd >> konsole >> fish >> sudo >> sudo >> L1E1.bin >> L1E1.bin(4617)(4618)(4619)

# Exercise 2
1. Execution time ~50 milliseconds
2. the difference is 735 us
3. the difference is 8623 us (arriveal time delta 10 us)

# Exercise 3
1. the lenght is ~11957 us when two are working and ~7971 us when three is working .
2. the lenght from start to start on red is 1397101 us
3. the lenght between is 1002899 us
4. the stress lenght is ~204 us. Because it's a big operation that takes a lot of time and processing power semi-intentionally creating a delay time before the clock can be assest.
5. uses 1 core about 45 to 55% of the cpu and the threads use up to 20 % of the cpu.

# Exercise 4
1) The red and green blinks in 1,2 sec periods as expected, but the blue is blinks in 1 sec periods times per second at all.
2) red: 1638841 us, green: 1642899 us, the blue: 1008696 us

# Exercise 5 
1. nanosleep is more precise than usleep as it measures time in nanoseconds, and clock_nanosleep allows you to choose between different clocks. In our case we use a absolute clock, which measures time since 1970 not since the command was run.
2. the difference is 18 nanoseconds.
3. drifing.

# Exercise 6
1.
