# BlockReader
BlockReader is a C++ reader(/parser) for Bitcoin binaries blk*.dat. The output is stored to a file (to specify as the last optionnal argument) in a pseudo-JSON format in order to make it readable. 

## Other files/directories
A src folder with the main and classes files.
A [data](https://github.com/darosior/BlockReader/tree/master/data) folder is provided with some sample blk files (the 2 firsts).  
A reader.bin executable.  
A result.txt : an example of the result for the reading of the first 150 blocks.  

## Usage   
```shell
./reader.bin "directory which contains binaries" <"output file"|"debug"> "nb of blocks to read" ("starting block")
```
If you specify debug as output File, result will be printed on the prompt.  
If you specify a starting block, results will be __printed__ from this block. Caution : as far as it is not possible to know the length of each before reading it, the program will read each block __even if you sepcify a starting one__ -> it could take a while if you specify a high-heighted block.
  
  

#### To do
(Compute Difficulty from target)  
