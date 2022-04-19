## This is a red-black tree with functions of finding k-th statistic and number of elements less than n (provided by user)

### To build the project the following is needed to be done:

```
mkdir build && cd build/ 
cmake .. && make
```

Adding sanitizers to main program and tests, if needed:

```
-DADD_SANITIZERS=1
```

To run main program:

```
make run_main
```

In main program you may use three following commands:

```
k num -------> to insert a number
m num -------> to get num-th least element in the tree
n num -------> to get number of elements less than num
```

Example of input:

```
k 8 k 2 k -1 m 1 m 2 n 3
```

Example of output:

```
-1 2 2
```

To run tests:

```
make run_tests
```

To run both main program and tests:

```
make run_all
```
