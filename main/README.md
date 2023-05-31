To run, you will need,
cmake
g++

To compile the code enter the following command,
```bash
cmake -S . -B build          #creates makefile and build info
cd build
make                    #compiles the code
./open_eex              #Runs the executable
cd tests && ctest    #Runs the tests
```