appName="plainIMG"
autorun=false
yesconsole=false
static=false

while getopts "vd" opt; do
  case ${opt} in
    ar ) autorun=true ;;
    yc ) yesconsole=true ;;
    st ) static=true ;;
    \? ) echo -e "\nUsage: $0 [-ar] [-yc] [-st]\n-ar | autorun    | autoruns the built executable\n-yc | yesconsole | removes the -mwindows flag from gcc, which is on by default\n-st | static     | adds the -static flag to gcc" ; exit 1 ;;
  esac
done

# you NEED to have dlls in the same directory
cp SDL3/bin/SDL3.dll .
cp SDL3_Image/bin/SDL3_image.dll .

args=()
! $yesconsole && args+=("-mwindows")
$static && args+=("-static")

gcc -o "${appName}.exe" main.c -I ./SDL3/include -I ./SDL3_Image/include -L ./SDL3/lib -L ./SDL3_Image/lib -l SDL3 -l SDL3_Image "${args[@]}"
if $autorun; then
    "./${appName}.exe"
fi

# test1-7.c
# gcc -o a.exe test4.c -I./SDL3/include -L./SDL3/lib -lSDL3
# gcc -o a.exe test4.c -I SDL3/include -L SDL3/lib -l SDL3
# gcc -o a.exe test6.c -I SDL3/include -I SDL3_Image/include -L SDL3/lib -L SDL3_Image/lib -l SDL3 -l SDL3_Image
# gcc -o a.exe test11.c -I SDL3/include -I SDL3_Image/include -L SDL3/lib -L SDL3_Image/lib -l SDL3 -l SDL3_Image -mwindows
# -static
# gcc -o plainIMG.exe main.c -I ./include -I SDL3/include -I SDL3_Image/include -L SDL3/lib -L SDL3_Image/lib -l SDL3 -l SDL3_Image -mwindows