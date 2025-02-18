VERSION=$1
git checkout main
git tag -a "$VERSION" -m "Release $VERSION"