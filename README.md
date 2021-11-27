# 📚 ft_containers
Re-implement the various container types of the C++ STL.

```
 |_ Makefile
 |___ includes (My container template files)
 |___ tester
       |___ OwnTester
       |___ mli42Tester (Thankyou for mli42)
```

## Test

`make all`
> test OwnTester & mli42Tester for Mandatory part (vector, stack, map)

`make own`
> test OwnTester Mandatory part

`make mli`
> test mli42Tester Mandatory part

`make bonus`
> test OwnTester & mli42Tester for Bonus part (set)

`make vector/stack/map/set`
> test OwnTester for each container

`make time`
> time compare for my containers and standard containers

`make print`
> print Red-Black Tree from print_RBtree.cpp file (in OwnTester)
