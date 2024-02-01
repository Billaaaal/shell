
#!/bin/bash

directories="bin dev lib lib64 lost+found mnt proc run snap sys usr boot etc init lib32 libx32 media opt root sbin srv tmp var"

for dir in $directories; do
    mkdir -p "./disk/$dir"
    echo "Created directory: $dir"
done
