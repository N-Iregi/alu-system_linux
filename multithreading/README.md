# multithreading

POSIX threads exercises: thread entry points, a thread-safe logger,
a multithreaded Gaussian blur, and a thread pool for prime factorization.

| File                | Description                                   |
|---------------------|-----------------------------------------------|
| 0-thread_entry.c    | Thread entry that prints a string, then exits |
| 1-tprintf.c         | printf prefixed with the thread ID            |
| 10-blur_portion.c   | Gaussian blur of one image portion            |
| 11-blur_image.c     | Whole-image blur using 8 threads              |
| 20-tprintf.c        | tprintf protected by a mutex (ctor/dtor)      |
| 21-prime_factors.c  | Prime factorization into a list               |
| 22-prime_factors.c  | Task creation/destruction and thread-pool exec|
