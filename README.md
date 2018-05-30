# BlockReader
BlockReader is a C++ reader(/parser) for Bitcoin binaries blk*.dat. The output is stored to a file (to specify as the last optionnal argument) in a pseudo-JSON format in order to make it readable. 

## Other files/directories
A src folder with the main and classes files.
A [data](https://github.com/darosior/BlockReader/tree/master/data) folder is provided with some sample blk files (the 2 firsts).  
A reader.bin executable.  
A result.txt : an example of the result for the reading of the first 150 blocks.  

## Usage   
```shell
./reader.bin "directory which contains binaries" "nb of blocks to read" <"output file"|"debug">
```
If you specify debug as output File, result will be printed on the prompt.  

**_Not finished yet._**

#### To do
Compute Hash for inputs  
Compute Difficulty from target  
Handle the case where the first part of a block is in file blk000(n) and the second in blk000(n+1)
