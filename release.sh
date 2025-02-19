plainimg_version=$(cat VERSION.txt)

./build.sh -r
./build.sh -s -r

strip --strip-all "build/release/static/plainIMG_static.exe"

iscc installer.iss

if [ ! -d "build/release/upload" ]; then
    mkdir -p build/release/upload
fi

cp "build/release/installer/plainIMG_installer.exe" "build/release/upload/plainIMG_v${plainimg_version}_installer.exe"
cp "build/release/static/plainIMG_static.exe" "build/release/upload/plainIMG_v${plainimg_version}_onefile.exe"
powershell Compress-Archive -Force -Path "build\release\dynamic\*" -DestinationPath "build\release\upload\plainIMG_v${plainimg_version}_dynamic.zip"

echo "release build complete"

#zip -r "build/release/upload/plainIMG_dynamic.zip" -j "build/release/dynamic"
# sorry tar.gz users
#zip -r "$OUTPUT_DIR/myprogram-${VERSION}.zip" -j "$OUTPUT_DIR/myprogram"
#tar -czvf "$OUTPUT_DIR/myprogram-${VERSION}.tar.gz" -C "$OUTPUT_DIR" myprogram
#sha256sum "$OUTPUT_DIR"/* > "$OUTPUT_DIR/checksums.sha256"
#iscc /DAppVersionArg="$VERSION" "installer.iss"
# https://stackoverflow.com/questions/28743736/inno-command-line-compilation-not-working
# mkdir -p build/upload