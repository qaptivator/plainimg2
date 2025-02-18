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

# you NEED to have dlls in the same directory if its not static
if ! $static; then
  cp src/SDL3/bin/SDL3.dll .
  cp src/SDL3_Image/bin/SDL3_image.dll .
fi

windres -i src/plainIMG.rc --input-format=rc -o src/plainIMG_rc.res -O coff

args=()
! $yesconsole && args+=("-mwindows")
$static && args+=("-lwinmm" "-lole32" "-lsetupapi" "-limm32" "-lversion" "-loleaut32" "-luuid" "-lmfplat")

# ohhh so i cant use spaces here
if $static; then
  gcc src/main.c src/${appName}_rc.res -o "${appName}-standalone.exe" -static -I./src/SDL3/include -I./src/SDL3_Image/include -L./src/SDL3/lib -L./src/SDL3_Image/lib -lSDL3 -lSDL3_image -lComdlg32 -lGdi32 "${args[@]}"
  if $autorun; then
    "./${appName}-standalone.exe"
  fi
else
  gcc src/main.c src/${appName}_rc.res -o "${appName}.exe" -I./src/SDL3/include -I./src/SDL3_Image/include -L./src/SDL3/lib -L./src/SDL3_Image/lib -lSDL3 -lSDL3_image -lComdlg32 -lGdi32 "${args[@]}"
  if $autorun; then
    "./${appName}.exe"
  fi
fi