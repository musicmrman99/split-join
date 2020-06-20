# Split-Join
A fast implementation of the basic functionality of python's slice operator.

## History
Originally created 2016-01-16, possibly with further development in July-August 2016.

## Building
Building should be as simple as (from the project root directory):
```
$ mkdir build
$ g++ -std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors src/main.cpp -o build/split-join
```

This has never been specifically tested on Windows or Mac, but I see no reason it shouldn't work. If it doesn't - open an issue!

## Examples
Basic usage:
```sh
$ split-join '/' 1:4 "this/is/my/only/string/forlong/yay"
is/my/only
```

If you give it an 'empty' range, it will produce an empty string:
```sh
$ split-join '/' 0:0 "this/is/my/only/string/forlong/yay"

```

Negative indices are allowed - they go from the end:
```sh
$ split-join '/' 0:-1 "this/is/my/only/string/forlong/yay"
this/is/my/only/string/forlong
$ split-join '/' -1: "/this/is/my/only/path/forlong/yay.ext"
yay.ext
```

You don't have to give a start or end - it defaults to the start and end of the string:
```sh
$ split-join '/' 3: "/this/is/my/only/path/forlong/yay.ext"
my/only/string/forlong/yay.ext
$ split-join '/' :3 "/this/is/my/only/path/forlong/yay.ext"
/this/is
$ split-join '/' : "/this/is/my/only/path/forlong/yay.ext"
/this/is/my/only/path/forlong/yay.ext
```

And of course, you can use outputs as inputs:
```sh
$ path="/this/is/my/only/path/forlong/yay.ext"
$ filename="$(split-join '/' -1: "$path")"
$ name="$(split-join '.' :-1 "$filename")"
$ extension="$(split-join '.' -1: "$filename")"
$ 
$ printf '%s\n' "$name" "$extension"
yay
ext
```
