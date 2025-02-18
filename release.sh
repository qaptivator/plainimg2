VERSION=$1

./build.sh -r
./build.sh -s -r
#iscc /DAppVersionArg="$VERSION" "installer.iss"
iscc installer.iss
# https://stackoverflow.com/questions/28743736/inno-command-line-compilation-not-working

# mkdir -p build/upload
if [ ! -d "build/release/upload" ]; then
    mkdir -p build/release/upload
fi

strip --strip-all "build/release/static/plainIMG_static.exe"

cp "build/release/installer/plainIMG_installer.exe" "build/release/upload/plainIMG_installer.exe"
cp "build/release/static/plainIMG_static.exe" "build/release/upload/plainIMG_onefile.exe"
powershell Compress-Archive -Force -Path "build\release\dynamic\*" -DestinationPath "build\release\upload\plainIMG_dynamic.zip"
#zip -r "build/release/upload/plainIMG_dynamic.zip" -j "build/release/dynamic"
# sorry tar.gz users

echo "release build complete"

# Zip versions
#zip -r "$OUTPUT_DIR/myprogram-${VERSION}.zip" -j "$OUTPUT_DIR/myprogram"
#tar -czvf "$OUTPUT_DIR/myprogram-${VERSION}.tar.gz" -C "$OUTPUT_DIR" myprogram

# Generate checksum
#sha256sum "$OUTPUT_DIR"/* > "$OUTPUT_DIR/checksums.sha256"

#echo "Release artifacts are in $OUTPUT_DIR"