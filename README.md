# Graph Layout library

This library was created for testing different graph layout algorithmes,
basically the force-based ones. For example, here implemented the
"Springs and Electrons" algorithm (*i can't remember how it is called for real
now*). Yep, that algorithm where you imagine that the edges of the graph are
the springs and the nodes of the graph are the electrons. So, the connected
nodes are attracting to each other, and the other ones are repulse from each other.

Look at `src/graphcalculator.cpp` for more. (Links are coming soon)

## Usage

### Requirements & dependencies

Currently this library depends on Qt Framework (strictly speaking, on some
libraries from Qt), but I'm going to get rid of that dependency soon. See
**issues** for more information.

#### Core dependencies

Core dependencies are necessary to build the main part of this library.

Currently here'll be described those versions of tools which I used to build
this project. They may not be the minimal required versions.

* Build automation tool: **CMake** 3.14.
* **Qt Framework** 5.12
* C++11 compliant compiler. Currently tested on **g++ 9.3.0** and **MSVC 2017**

#### QML dependencies

If you would like to test or to use the QML library for graph representation,
you will need the following modules:

* **QtQuick** 2.0
* **QtQuick.Shapes** 1.12
* **QtQuick.Models** 2.12
* **QtQml** 2.0

### How to use in your project

To add this library to your project now you have the only option: clone this
repository (don't forget `--recurse-submodules``) and copy it to your project
or add this repo as submodule into your project repository.

Then just add `add_subdirectory(graph-layout)` to your `CMakeLists.txt` file
and link your project to `graphlayout` library.

#### Usage of QML library

As an example of graph representation you can use the QML library shipped with
this project. To use them in your project you'll need to follow the steps above,
but also you should do some additional steps.

First of all, this library sets the `GRAPH_LAYOUT_QML_RESOURCES` CMake variable.
You should add this variable into the list of your sources like here:

```cmake

add_executable(my_very_very_project
	main.cpp
	shit.cpp
	test.cpp
	${GRAPH_LAYOUT_QML_RESOURCES}
)
```

And also you'll need to add the following line into your project code (somewhere
where you show your QML):

```cpp

// For example, we're using the QQuickView class

QQuickView view;
view.engine()->addImportPath("qrc:///");	// This line should be added
```

#### Example

You can check the example project using this library at [graph-layout-gui](https://github.com/Kickoman/graph-layout-gui/) repo.

## Documentation

Coming soon.
