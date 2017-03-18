A brief after-action report of your homework, as a text file readme.txt. This should 
discuss any issues that you ran into, and your conclusions about how well your 
implementation of SRT improves its performance.

When viewing the main.c file, I observed that the each pixel was being worked on independently.
I tried parallelizing the inner loop but I came upon the conclusion that it may be more
convenient to parallelize the outer loop. My reasoning for this was to avoid having to pass
in x and dx in as additional variables in the struct pointer passed, which runs the risk
of generating unintended bugs.

The way I split up the work among the threads was to store the number of rows that each thread 
would handle in an int called width_for_thread, where width_for_thread = (width/nthreads). 
Having this, I ran a for loop from 0 to nthreads-1 and I created a thread for every iteration. 
The struct I passed into the pthread_create method has two variables, starting row, and ending row.
Using this, I reassigned the starting row as (width_for_thread*i) and the ending row as 
(width_for_thread*(i+1)) during each iteration.

An error I stumbled upon while working on this section was that because I was using the same 
struct pointer for each thread, my rendered image was missing horizontal portions of the image. 
I realized this was due to race conditions and to fix this I altered passing in a single struct 
for each iteration of the loop, to passing in an element of an array of structs into the 
pthread_create method. Also another bug I found was that casting the address of my struct passed
in, into a (void *) would cause the image to obtain the wrong values. Without alteration of
the address, the code worked fine.

I ran the loop of creating threads only up to nthreads-1 because I wanted the last thread to
work on the rows that it is responsible for AND the remaining rows in the image. Thus, for
an image with 131 rows, using 2 threads would mean the first thread, taken care of by the for
loop, would work on rows 0-64, and the second thread would work on 65-129, and the remaining row
130.

For the variables I made global, I changed the scene used, and also added a 3-D array with
the dimensions of the x and y positions, and slots for each pixel's respective RGB values.
Only after I joined all the threads, I looped through each element and printed out the
colors in their intended order. I made the program and tested with ./srt 1, 2, 4, 8 before
running make clean check in order to make sure my code was working properly

Checking my modified code, I was able to find the following performance improvements:

time ./srt 1 > /dev/null
real	0m42.915s
user	0m42.912s
sys		0m0.002s

time ./srt 2 > /dev/null
real	0m22.783s
user	0m43.758s
sys		0m0.001s

time ./srt 4 > /dev/null
real	0m14.991s
user	0m44.246s
sys		0m0.002s

time ./srt 8 > /dev/null
real	0m9.384s
user	0m44.303s
sys		0m0.002s

Reading these performance values, it is apparent that using a higher number of threads
greatly improves real time performance. For ./srt 1, real time = user time + sys time
which is to be expected. With an increasing number of threads, less real time is taken
as different threads are performing their respective tasks concurrently. By ./srt 8,
the real time taken has been reduced by approximately a factor of 5.