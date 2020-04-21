## Motivation
Some IDE may not support creation of the .inl file, that already have include guards/pragma.
Run this script to create necessary files.
For more information run
``` cpp
./crilf --help
```

### Example
``` cpp
./crilf -pf temp -p temp_1 temp_2
```

The result of this :
1) Folder named "temp" within "temp.inl"(with pragma once) inside.
2) File "temp_1.inl"(with pragma once) inside.
3) Folder named "temp_2" within "temp_2.inl"(with include guards) inside.