# SparseBase

## Compiling
```
mkdir build && cd build
cmake ..
make
```

This will generate the library as a static library. In addition, the example codes are compiled and their binaries are located in `build/examples`.

## Installation

To install the library, compile the library as shown in the previous section. Afterwards, you can install the library files either to the systems global location or to a custom location. To install the library to the default system location:
```
cd build
cmake --install .
```

To install the library to a custom directory, do the following:
```
cd build
cmake --install . --prefix "/custom/location"
```
**Note:** We suggest using **absolute paths** for prefixes as relative paths have been known to cause problems from time to time.

## Usage

When compiling a project that uses SparseBase, simply link the project with the library using the flag `-lsparsebase`.

## Tests

Users can run unit tests easily after building the project. To do so, they must configure CMake to compile tests:
```
mkdir build # if a build directory doesn't exist
cd build
cmake -DRUN_TESTS ..
```
Once its built, while in the build directory, do the following:
``` 
ctest -V
```

# Getting Started

## Input

Currently we support two sparse data file formats:
- Matrix Market Files (.mtx)
- Undirected Edge List Files (.uedgelist)

We can perform a read operation on these formats as shown below: (use `UEdgeListReader` fro .uedgelist files)
```c++
auto reader = new MTXReader<vertex_type, edge_type, value_type>(file_name);
auto data = reader->read();
```

The data object retrieved here is a vector of SparseFormats. If the file contains a single SparseFormat (tensor, matrix etc.),
then we can simply take the 0-th index of this.
```c++
auto result = data[0];
```

Currently, both readers only support a single SparseFormat per file. But we have plans to change this.

## Converting Formats

Readers might use different formats. So we can not be sure about the format of the final result of the read operation.

We can convert the data into the format we desire using ``SparseConverter``:
```c++
auto converter = SparseConverter<vertex_type, edge_type, value_type>();
auto converted = converter.convert(result, CSR_f);
auto csr = dynamic_cast<CSR<vertex_type, edge_type, value_type>>(converted);
```

## Working with Graphs

Graphs can be created from a vector of formats. So we can use the output of the readers directly.

```c++
auto reader = new MTXReader<vertex_type, edge_type, value_type>(file_name);
auto data = reader->read();
auto g = Graph<vertex_type, edge_type, value_type>(data);
```

As of the current version of the library graphs function as containers of sparse data. There are plans to change this however in future releases.

## Ordering

Ordering can be handled using the ``ReorderInstance`` class. Note that this class takes a ``ReorderPreprocessType`` as a template parameter.
As of the current version two such ``ReorderPreprocessType`` classes exist ``RCMReorder`` and ``DegreeReorder``.

Below you can see an example of an RCM reordering of a graph.
```c++
ReorderInstance<vertex_type, edge_type, value_type, RCMReorder<vertex_type, edge_type, value_type>> orderer;
SparseFormat<vertex_type, edge_type, value_type> * con = g.get_connectivity();
vertex_type * order = orderer.get_reorder(con);
```

For these operations, we also support an alternative syntax (without the template parameter) using the ``RCMReorderInstance`` and ``DegreeReorderInstance`` wrapper classes.

Below you can see this alternative syntax being used to reorder the same graph.
```c++
RCMReorderInstance<vertex_type, edge_type, value_type> orderer;
SparseFormat<vertex_type, edge_type, value_type> * con = g.get_connectivity();
vertex_type * order = orderer.get_reorder(con);
```


# Contribution Guidelines

Contributions preferably start with an issue on the issue tracker of GitHub. In addition, a contribution of any kind must be forked out of `origin/develop` and merged back into it. 

TL;DR: the process for making a contribution is to make a topic branch out of `origin/develop` into your local machine, make your contributions on this topic branch, push your new branch back into `origin`, and create a pull request to pull your new topic branch into `origin/develop`. Please do not merge your changes to `develop` on your local machine and push your changes to `origin/develop` directly. 

More precisely, a typical contribution will follow this pattern:

1. Create an issue on GitHub discussing your contribution. At this point, a discussion may happen where the entire team can get on the same page.
2. Pull `origin/develop` into your local to start developing from the latest state of the project, and create a new branch for your contribution. The naming convention for a contribution branch is `feature/<new_feature>`:
    
    ```bash
    # on your local
    cd sparsebase
    git checkout develop
    git pull origin develop
    git checkout -b feature/<new_feature>
    ```
    
3. After you're done working on your feature, make sure that it can be merged cleanly with `origin/develop` by pulling `origin/develop` back into your local machine and merging it with your feature branch:
    
    ```bash
    git checkout develop
    git pull origin develop
    git checkout feature/<new_feature>
    git merge develop
    # merge conflicts may arise
    ```
    
4. Once your feature branch merges successfully with `develop`, push your branch to `origin`:
    
    ```bash
    git checkout feature/<new_feature>
    git push origin feature/<new_feature>
    ```
    
5. On GitHub, create a pull request to merge your branch with `develop`; the base of the request will be `develop` and the merging branch will be `feature/<new_feature>`.
6.  Once the contribution is reviewed, a maintainer from the team will merge the pull request into `origin/develop`.

Thank you for your efforts!
