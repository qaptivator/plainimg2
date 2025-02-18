appName="plainIMG"
autorun=false
yesconsole=false
static=false

while getopts "hays" OPTION; do
  case $OPTION in
    a) autorun=true ;;
    y) yesconsole=true ;;
    s) static=true ;;
    h) echo -e "\nUsage: $0 [-a] [-y] [-s]\n-a | autorun    | autoruns the built executable\n-y | yesconsole | removes the -mwindows flag from gcc, which is on by default\n-s | static     | adds the -static flag to gcc" ; exit 1 ;;
    \?) echo "Invalid option. See the help menu with '-h'" >&2; exit 1;;
  esac
done

# you NEED to have dlls in the same directory
if ! $static; then
  cp src/SDL3/bin/SDL3.dll .
  cp src/SDL3_Image/bin/SDL3_image.dll .
fi

#if [ ! -f "./src/plainIMG_rc.o" ]; then
#windres src/plainIMG.rc src/plainIMG_rc.o
#fi

windres -i src/plainIMG.rc --input-format=rc -o src/plainIMG_rc.res -O coff

args=()
! $yesconsole && args+=("-mwindows")
$static && args+=("-lwinmm" "-lole32" "-lsetupapi" "-limm32" "-lversion" "-loleaut32" "-luuid" "-lmfplat")

# "-l winmm" "-l ole32" "-l setupapi" "-l imm32" "-l version" "-l oleaut32" "-l uuid" "-l mfplat"
# -l winmm -l ole32 -l setupapi -l imm32 -l version -l oleaut32 -l uuid -l mfplat

# ohhh so i CANT use spaces here
if $static; then
  gcc -o "${appName}-standalone.exe" src/plainIMG_rc.res src/main.c -static -I./src/SDL3/include -I./src/SDL3_Image/include -L./src/SDL3/lib -L./src/SDL3_Image/lib -lSDL3 -lSDL3_image -lComdlg32 -lGdi32 "${args[@]}"
  #if [ -f "./src/plainIMG_rc.res" ]; then
  #else
  #  echo "sorry but you NEED to have the resource file built in src called 'plainIMG_rc.o', because it is required by the program to run."
  #  exit 1
  #  #gcc -o "${appName}-standalone.exe" src/main.c -static -I./src/SDL3/include -I./src/SDL3_Image/include -L./src/SDL3/lib -L./src/SDL3_Image/lib -lSDL3 -lSDL3_image -lComdlg32 "${args[@]}"
  #fi
else
  gcc -o "${appName}.exe" src/plainIMG_rc.res src/main.c -I./src/SDL3/include -I./src/SDL3_Image/include -L./src/SDL3/lib -L./src/SDL3_Image/lib -lSDL3 -lSDL3_image -lComdlg32 -lGdi32 "${args[@]}"
  #if [ -f "./src/plainIMG_rc.res" ]; then
  #  
  #else
  #  echo "sorry but you NEED to have the resource file built in src called 'plainIMG_rc.o', because it is required by the program to run."
  #  exit 1
  #  #gcc -o "${appName}.exe" src/main.c -I./src/SDL3/include -I./src/SDL3_Image/include -L./src/SDL3/lib -L./src/SDL3_Image/lib -lSDL3 -lSDL3_image -lComdlg32 "${args[@]}"
  #fi
fi

if $autorun; then
    "./${appName}.exe"
fi

# build the icon with "windres plainIMG.rc -o plainIMG_rc.o"

# test1-7.c
# gcc -o a.exe test4.c -I./SDL3/include -L./SDL3/lib -lSDL3
# gcc -o a.exe test4.c -I SDL3/include -L SDL3/lib -l SDL3
# gcc -o a.exe test6.c -I SDL3/include -I SDL3_Image/include -L SDL3/lib -L SDL3_Image/lib -l SDL3 -l SDL3_Image
# gcc -o a.exe test11.c -I SDL3/include -I SDL3_Image/include -L SDL3/lib -L SDL3_Image/lib -l SDL3 -l SDL3_Image -mwindows
# -static
# gcc -o plainIMG.exe main.c -I ./include -I SDL3/include -I SDL3_Image/include -L SDL3/lib -L SDL3_Image/lib -l SDL3 -l SDL3_Image -mwindows