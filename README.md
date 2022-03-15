## This is a red-black tree with functions of finding k-th statistic and number of elements less than n (provided by user)

### To build the project the following is needed to be done:

```
mkdir build && cd build/ 
cmake .. && make
```

Adding sanitizers to main program, if needed (sanitizers are already enabled for the tests):

```
-DADD_SANITIZERS=1
```

To run main program:

```
make run_main
```

To run tests:

```
make run_tests
```

To run both main program and tests:

```
make run_all
```