# Matrix-Multiplications-threads-
Objectives
1. Introducing threads concepts and POSIX threads library.
2. Implementing popular algorithms as multi-threaded ones.
Problem Statement
It is required to implement two popular algorithms as multi-threaded ones. They are as
follows:
Matrix Multiplication
It is required to implement two variations of this algorithm:
1. The computation of each element of the output matrix happens in a thread.
2. The computation of each row of the output matrix happens in a thread.
For both variations, you have to compute the elapsed time for each of them, compare
them and justify your answer.
The program should read two input matrices from a file in the following format:
[number of
1st matrix
[number of
2nd matrix
rows of 1st matrix] [number of columns of 1st matrix]
entries
rows of 2nd matrix] [number of columns of 2nd matrix]
entries
Note: [] for clarity
example input:

3 5

1 -2 3 4 5

11 2 -3 4 5

-1 2 3 4 5

5 4

-1 2 3 4

1 -2 3 4

1 2 -3 4

1 2 3 -4

-1 -2 -3 -4

output format:
[result matrix
END1 [elapsed
[result matrix
END2 [elapsed
entries]
time of procedure 1]
entries]
time of procedure 2]
Once again please note that [] is for clarity you shouldn't print it
