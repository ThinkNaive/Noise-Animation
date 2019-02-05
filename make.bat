clang++ -g mask.cpp noise.cpp -o noise.exe -Wall -static-libgcc --target=x86_64-w64-mingw -std=c++11 -DGLUT_DISABLE_ATEXIT_HACK -Wl,--subsystem,windows -lglut32 -lglu32 -lopengl32
gcc -o bmp2bin mask2bin.cpp bmp2mask.cpp
