# Split-Join
A fast implementation of the basic functionality of python's slice operator.

Originally created 2016-01-16, possibly with further development in July-August 2016.

Basic usage:
```sh
$ ./split-join '/' 1:4 "this/is/my/only/string/forlong/yay"
is/my/only
```

If you give it an 'empty' range, it will produce an empty string:
```sh
$ ./split-join '/' 0:0 "this/is/my/only/string/forlong/yay"

```

Negative indeces are allowed - they go from the end:
```sh
$ ./split-join '/' 0:-1 "this/is/my/only/string/forlong/yay"
this/is/my/only/string/forlong
$ ./split-join '/' -1: "/this/is/my/only/path/forlong/yay.ext"
yay.ext
```

You don't have to give a start or end - it defaults to the start and end of the string:
```sh
$ ./split-join '/' 3: "/this/is/my/only/path/forlong/yay.ext"
my/only/string/forlong/yay.ext
$ ./split-join '/' :3 "/this/is/my/only/path/forlong/yay.ext"
/this/is
$ ./split-join '/' : "/this/is/my/only/path/forlong/yay.ext"
/this/is/my/only/path/forlong/yay.ext
```
