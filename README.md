# BlockReader
BlockReader is a simple C++ reader(/parser) for Bitcoin binaries blk*.dat. You can store the output to a file (to specify as the last optionnal argument). The standard output displays more (meaningless) details. 

## Other files/directories
A [data](https://github.com/darosior/BlockReader/tree/master/data) folder is provided with some sample blocks (the 2 firsts).  
The [test.txt](https://github.com/darosior/BlockReader/blob/master/test.txt) file is the result output from the data directory.  

## Usage   
```
./bin "directory which contains binaries" "nb of blocks to read" ("output file")  
```
