VERSION=$1

./build.sh -r
./build.sh -s -r
iscc.exe installer.iss

echo "release build complete"

# Zip versions
#zip -r "$OUTPUT_DIR/myprogram-${VERSION}.zip" -j "$OUTPUT_DIR/myprogram"
#tar -czvf "$OUTPUT_DIR/myprogram-${VERSION}.tar.gz" -C "$OUTPUT_DIR" myprogram

# Generate checksum
#sha256sum "$OUTPUT_DIR"/* > "$OUTPUT_DIR/checksums.sha256"

#echo "Release artifacts are in $OUTPUT_DIR"