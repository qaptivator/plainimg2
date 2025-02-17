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
cp src/SDL3/bin/SDL3.dll .
cp src/SDL3_Image/bin/SDL3_image.dll .

args=()
! $yesconsole && args+=("-mwindows")
$static && args+=("-static")

if [ -f "./src/plainIMG_rc.o" ]; then
  gcc -o "${appName}.exe" src/plainIMG_rc.o "${args[@]}" src/main.c -I ./src/SDL3/include -I ./src/SDL3_Image/include -L ./src/SDL3/lib -L ./src/SDL3_Image/lib -l:libSDL3.dll.a -l:libSDL3_image.dll.a
else
  gcc -o "${appName}.exe" "${args[@]}" src/main.c -I ./src/SDL3/include -I ./src/SDL3_Image/include -L ./src/SDL3/lib -L ./src/SDL3_Image/lib -l:libSDL3.dll.a -l:libSDL3_image.dll.a
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