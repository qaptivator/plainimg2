appName="plainIMG"
autorun=false
yesconsole=false
static=false
release=false
profile=false

plainimg_version=$(cat VERSION.txt)

while getopts "hprays" OPTION; do
  case $OPTION in
    a) autorun=true ;;
    y) yesconsole=true ;;
    s) static=true ;;
    r) release=true ;;
    p) profile=true ;;
    h) echo -e "\nUsage: $0 [-a] [-y] [-s] [-r] [-p]\n-a | autorun    | autoruns the built executable\n-y | yesconsole | removes the -mwindows flag from gcc, which is on by default\n-s | static     | adds the -static flag to gcc (builds standalone)" ; exit 1 ;;
    \?) echo "Invalid option. See the help menu with '-h'" >&2; exit 1;;
  esac
done

args=()
! $yesconsole && args+=("-mwindows")
$static && args+=("-lwinmm" "-lole32" "-lsetupapi" "-limm32" "-lversion" "-loleaut32" "-luuid" "-lmfplat")
$profile && args+=("-no-pie" "-pg") #-g -pg
#https://stackoverflow.com/questions/42620074/gprof-produces-empty-output
# to profile, run "./build.sh -s -p", "cd build/debug", "./plainIMG_static.exe", "gprof plainIMG_static.exe gmon.out > analysis.txt"

windres -i src/plainIMG.rc --input-format=rc -o src/plainIMG_rc.res -O coff

if [ ! -d "build" ]; then
  mkdir build
fi

if ! $release; then
  if [ ! -d "build/debug" ]; then
    mkdir build/debug
  fi

  # you NEED to have dlls in the same directory if its not static
  if ! $static; then
    if [ ! -f "build/debug/SDL3.dll" ]; then
      cp src/SDL3/bin/SDL3.dll build/debug
    fi
    if [ ! -f "build/debug/SDL3_image.dll" ]; then
      cp src/SDL3_Image/bin/SDL3_image.dll build/debug
    fi
  fi

  # use $var to reference a variable, use $(command) to run a command, use ${var} to referece a variable, in case theres a letter infront of it
  # ohhh so i cant use spaces here
  if $static; then
    gcc -m64 -DPLAINIMG_VERSION=\"${plainimg_version}\" src/main.c src/${appName}_rc.res -o "build/debug/${appName}_static.exe" -static -I./src/SDL3/include -I./src/SDL3_Image/include -L./src/SDL3/lib -L./src/SDL3_Image/lib -lSDL3 -lSDL3_image -lComdlg32 -lGdi32 "${args[@]}"
    if $autorun; then
      "./build/debug/${appName}_static.exe"
    fi
  else
    gcc -m64 -DPLAINIMG_VERSION=\"${plainimg_version}\" src/main.c src/${appName}_rc.res -o "build/debug/${appName}.exe" -I./src/SDL3/include -I./src/SDL3_Image/include -L./src/SDL3/lib -L./src/SDL3_Image/lib -lSDL3 -lSDL3_image -lComdlg32 -lGdi32 "${args[@]}"
    if $autorun; then
      "./build/debug/${appName}.exe"
    fi
  fi
else
  if [ ! -d "build/release" ]; then
    mkdir build/release
  fi

  if ! $static; then
    if [ ! -d "build/release/dynamic" ]; then
      mkdir build/release/dynamic
    fi
    if [ ! -f "build/release/dynamic/SDL3.dll" ]; then
      cp src/SDL3/bin/SDL3.dll build/release/dynamic
    fi
    if [ ! -f "build/release/dynamic/SDL3_image.dll" ]; then
      cp src/SDL3_Image/bin/SDL3_image.dll build/release/dynamic
    fi
  fi

  # ohhh so i cant use spaces here
  if $static; then
    if [ ! -d "build/release/static" ]; then
      mkdir build/release/static
    fi
    gcc -DPLAINIMG_VERSION=\"${plainimg_version}\" -m64 src/main.c src/${appName}_rc.res -o "build/release/static/${appName}_static.exe" -static -I./src/SDL3/include -I./src/SDL3_Image/include -L./src/SDL3/lib -L./src/SDL3_Image/lib -lSDL3 -lSDL3_image -lComdlg32 -lGdi32 "${args[@]}"
    if $autorun; then
      "./build/release/static/${appName}_static.exe"
    fi
  else
    if [ ! -d "build/release/dynamic" ]; then
      mkdir build/release/dynamic
    fi
    gcc -DPLAINIMG_VERSION=\"${plainimg_version}\" -m64 src/main.c src/${appName}_rc.res -o "build/release/dynamic/${appName}.exe" -I./src/SDL3/include -I./src/SDL3_Image/include -L./src/SDL3/lib -L./src/SDL3_Image/lib -lSDL3 -lSDL3_image -lComdlg32 -lGdi32 "${args[@]}"
    if $autorun; then
      "./build/release/dynamic/${appName}.exe"
    fi
  fi
fi

echo "build complete"
