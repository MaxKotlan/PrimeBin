# PrimeBin

This program converts text files containing integers represented by ascii digits into binary files. Converting to binary files can result in smaller file sizes, and extremely fast loading (using sequential reads) and interpretation (mapping directly into memory). 

## Why Prime Bin?

I wrote this program to assist me with another project. I wanted to create a precomputed memory map of all the prime numbers under MAX_INT (4294967295).

## Example
Assume you have a text file (or any file) that contains the following:
  
    /*example.txt*/
    1
    23
    343
    4354

To convert this file to a memory map, you would execute the following

    ./primebin example.txt

This would generate a file called example.bin which would be structured as follows (assuming little endian and uint32):

    01 00 00 00 17 00 00 00 00 57 01 00 00 02 11 00 00

This file could be loaded directly into memory, without the need for any parsing.

## Delimiters and Other Bases 

Primebin will parse any sequential ascii digits. It does not matter how they are delimited. For instance

    1,2 3:4/56 a b c d e f
 will result in
 

    01 00 00 00 02 00 00 00 03 00 00 00 04 00 00 00 38 00 00 00
    (1)         (2)         (3)         (4)        (56)

Primebin will skip letters unless the base is greater than 10. For instance

    ./primebin --base 12 example.txt

This will result in

    01 00 00 00 02 00 00 00 03 00 00 00 04 00 00 00 38 00 00 00 0A 00 00 00 0B 00 00 00
    (1)         (2)         (3)         (4)        (56)        (10)        (11)

  ## Memory Usage
 Memory is cheap and I wrote this program for my personal needs. My personal needs required performance, over minimum memory usage, so the entire input file is read into memory with sequential reads.  If you're parsing a text file 2 gb in size, then you need at most 4gb of memory (usually less). If you want lower memory usage, request it. 

Sorry ¯\\_(ツ)_/¯