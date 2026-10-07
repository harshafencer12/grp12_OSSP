#!/bin/bash
set -e

rm -f original.txt hardlink.txt symlink.txt
printf "inode demonstration\n" > original.txt

echo "=== Original ==="
ls -li original.txt
stat original.txt

ln original.txt hardlink.txt
ln -s original.txt symlink.txt

echo
echo "=== After creating hard and symbolic links ==="
ls -li original.txt hardlink.txt symlink.txt

echo
echo "=== find by inode of original ==="
INODE=$(stat -c '%i' original.txt)
find . -maxdepth 1 -inum "$INODE" -printf '%f\n'

echo
echo "Hard link shares the same inode as the original."
echo "Symbolic link has its own inode and stores a pathname."
