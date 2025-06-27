# Lines of Code Counter

A fairly simple program that takes in files as command line arguments and outputs the total number of lines, code, comments, and blanks in the files.

## Usage

```
make
./loc main.cpp
```

Calling make will make the default build creating the `loc` executable. Then said executable is called with the argument of the desired file(s) (can use as many as you want). Here is the output for the previous example call:

```
Name    Total    Code    Comment    Blank    
------------------------------------------
main.cpp    46    30    6    10

```