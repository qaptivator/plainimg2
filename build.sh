# -static

# you NEED to have dlls in the same directory
cp SDL3/bin/SDL3.dll .
cp SDL3_Image/bin/SDL3_image.dll .

# test1-7.c
# gcc -o a.exe test4.c -I./SDL3/include -L./SDL3/lib -lSDL3
# gcc -o a.exe test4.c -I SDL3/include -L SDL3/lib -l SDL3
# gcc -o a.exe test6.c -I SDL3/include -I SDL3_Image/include -L SDL3/lib -L SDL3_Image/lib -l SDL3 -l SDL3_Image
# gcc -o a.exe test11.c -I SDL3/include -I SDL3_Image/include -L SDL3/lib -L SDL3_Image/lib -l SDL3 -l SDL3_Image -mwindows

gcc -o plainIMG.exe main.c -I ./include -I SDL3/include -I SDL3_Image/include -L SDL3/lib -L SDL3_Image/lib -l SDL3 -l SDL3_Image -mwindows