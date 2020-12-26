# TreeNode

TreeNode is a small C++ library that gives a class the behavior of a tree node. This allows to easily organize objects which have a hierarchy.

## Main features
 - Provide a tree node behavior to a class via inheritance
 - No dynamic allocations
 - Depth-First iterator

## Limitations

 - An instance can only belong to one tree graph at a time.
 - Children lifetime should always be shorter than parents (i.e. an
   instance should not be destructed before all of its children are
   destroyed).

## Tests

To build and run the tests:

```bash
mkdir build
cd build
cmake ..
make all test
```
