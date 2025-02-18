VERSION=$1

./build.sh -r
./build.sh -s -r

# Create build output directory
OUTPUT_DIR="release-$VERSION"
mkdir -p "$OUTPUT_DIR"

# Build (Modify this part based on your project)
echo "Building project..."
make release || { echo "Build failed!"; exit 1; }

# Package binaries
echo "Packaging..."
cp myprogram "$OUTPUT_DIR/"

# Zip versions
zip -r "$OUTPUT_DIR/myprogram-${VERSION}.zip" -j "$OUTPUT_DIR/myprogram"
tar -czvf "$OUTPUT_DIR/myprogram-${VERSION}.tar.gz" -C "$OUTPUT_DIR" myprogram

# Generate checksum
sha256sum "$OUTPUT_DIR"/* > "$OUTPUT_DIR/checksums.sha256"

echo "Release artifacts are in $OUTPUT_DIR"