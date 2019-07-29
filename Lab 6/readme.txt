1. wget https://web.cs.ucla.edu/classes/winter19/cs35L/assign/srt.tgz

Output : --2019-02-19 11:54:10--  https://web.cs.ucla.edu/classes/winter19/cs35L/assign/srt.tgz
Resolving web.cs.ucla.edu (web.cs.ucla.edu)... 131.179.128.29
Connecting to web.cs.ucla.edu (web.cs.ucla.edu)|131.179.128.29|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 52223 (51K) [application/x-gzip]
Saving to: ‘srt.tgz’

100%[======================================================================>] 52,223      --.-K/s   in 0.002s  

2019-02-19 11:54:10 (24.0 MB/s) - ‘srt.tgz’ saved [52223/52223]

2. tar -xf srt.tgz

Output : srt  srt.tgz  test.txt

3. cd srt

4. make 

Output : 

gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o main.o main.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o raymath.o raymath.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o shaders.o shaders.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter -o srt main.o raymath.o shaders.o -lm

6. Before I look into the main.c file in the srt directory. I carefully review the 
   the way to use the pthread_create, pthread_join from the lecture slides during the
   week. After review, I start look into the make.c and locate the operations
   that I need to move out from the main in order to change it into a multithread
   program. Then, I come across the large nested for loop that is responsible for
   the calculation of the pixels and the printing of the data for the pixel.
   I realize that this implementation should be the part that needs to be accessed
   by each thread. Therefore, I move that part of the implementation from the
   main out into a function called pixelOp. 

7. When I am moving the large nested for loop into the newly created function, I keep
   encountering the error of mismatch parentheses. After searching for awhile, I figure out that 
   I miss a closing parentheses for the outer for loop. 

8. After I move the implementation, then I find out that the variables nthreads and
   scence needs to be declared as global variables instead of leaving in the main
   as local variables. It is because both of the variables need to be accessed by 
   the main and the function pixelOp.  

9. Then, I read the comment that mention to comment put the below part and put the
   appropriate values into the array. I guess I have to somehow create an array in
   this modification, but I don’t know what kind of array and the needed size at that
   point. After that, I proceed to try to print out the output values. However, the
   output seems to be odd and it is out of format. Therefore, I find out that 
   I could solve this by making a three dimension array globally and this would solve 
   the final printing format.

9. According to the pthread_create example from the lecture slide, I create a for loop
   that is responsible for creating the number of threads which is given from the provided
   arguments. Also, I also create another for loop that is responsible to wait for the 
   completion of each thread by using pthread_join. At last, I modify the original print 
   function that is able to loop through the 3D array to print out the stored values.

10. Next, I try to run the “make clean check” command to check the output. However,
    I keep getting error about undefined reference to the 
    the pthread_create and pthread_join. I know it is not possible that the undefined 
    error relate to main.c since I already include the pthread library. 
    After re-read the lecture slides, I realize that I need to 
    modify the Makefile and make it link with the -lpthread flag.

    Original Part in Makefile : LDLIBS = -lm

    Error Output : 

rm -f *.o *.tmp 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm srt srt.tgz
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o main.o main.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o raymath.o raymath.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o shaders.o shaders.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter -o srt main.o raymath.o shaders.o -lm
main.o: In function `main':
/u/cs/ugrad/sum/lab6/srt/main.c:312: undefined reference to `pthread_create'
/u/cs/ugrad/sum/lab6/srt/main.c:324: undefined reference to `pthread_join'
collect2: error: ld returned 1 exit status
make: *** [Makefile:25: srt] Error 1
        
    Modify Part in Makefile : LDLIBS = -lm -lpthread

11. After that, I re-run the “make clean check” and obtain the following output. 

Command : make clean check

Output : 

rm -f *.o *.tmp 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm srt srt.tgz
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o main.o main.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o raymath.o raymath.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o shaders.o shaders.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter -o srt main.o raymath.o shaders.o -lm -lpthread
time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m42.808s
user	0m42.797s
sys	0m0.003s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real	0m21.103s
user	0m41.073s
sys	0m0.001s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real	0m14.974s
user	0m44.284s
sys	0m0.003s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real	0m9.324s
user	0m44.195s
sys	0m0.002s
mv 8-test.ppm.tmp 8-test.ppm
for file in 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm; do \
  diff -u baseline.ppm $file || exit; \
done

Observations : After the modification of the SRT implementation into multithreaded,
we can see as the number of threads increase from 1 to 8, the real time starts to
drastically decrease from 42.808 seconds to 9.324 seconds. Therefore, the multithreaded
implementation of the ray tracer code run several times faster than the original version.





