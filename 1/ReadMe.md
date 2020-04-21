## Motivation
You can find pack of textures/models/etc,
that (possibly)add a name of the pack at the beginning at each file.
(Example : https://www.spriters-resource.com/)

Run this script with the name of the pack to rename all files in the current directory.
New name will be the string without pack name.

Supposed, that you don't have conflict names that can appear after renaming.

### Example
``` cpp
./RenamePack "Heroes of Might and Magic 3"
```

Then all files(recursively down) will be renamed without this line.

