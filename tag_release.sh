plainimg_version=$(cat VERSION.txt)
git checkout main
git tag -a "v$plainimg_version" -m "Release v$plainimg_version"
git push origin tag "v$plainimg_version"