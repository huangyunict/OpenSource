This project is available at: http://www.chokkan.org/software/liblbfgs/

Version is liblbfgs-1.10.tar.gz

I give simple C++ wrap in directory ``libxx''.
The implementation only wraps the function pointer syntax by functional syntax. 
The wrapping implementation do NOT support SSE optimization.

[Note] I change some source codes:

include/lbfgs.h:484: add const modifier
lib/lbfgs.c:252: add const modifier 

[Build]
./configure
make

cd libxx; make; cd ..

