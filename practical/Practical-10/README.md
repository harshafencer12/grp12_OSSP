# Practical 10

## Inodes, hard links and symbolic links
```bash
chmod +x inode_links_demo.sh
./inode_links_demo.sh
```
Use `ls -i`, `stat`, and `find` to inspect inode numbers.

## mmap file I/O
```bash
gcc -Wall -Wextra mmap_file.c -o mmap_file
./mmap_file
cat mmap_demo.txt
```

`MAP_SHARED` allows writes through the mapping to be reflected in the underlying file (after synchronization as appropriate). Memory-mapped I/O avoids an explicit read/write loop for the mapped region, but introduces mapping and page-management considerations.
