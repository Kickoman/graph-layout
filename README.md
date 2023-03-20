# Graph Layout library

Linux build: ![linux build status](https://github.com/Kickoman/graph-layout/actions/workflows/build_linux.yml/badge.svg)

Windows build: ![windows build status](https://github.com/Kickoman/graph-layout/actions/workflows/build_windows.yml/badge.svg)

## Description

*Bielaruskaja versija nižej*

This library was created for testing different graph layout algorithms, particularly the force-based ones. 
For example, it implements the "Springs and Electrons" algorithm (I can't recall its actual name at the moment). 
This algorithm imagines the edges of the graph as springs and the nodes of the graph as electrons. Connected nodes 
attract each other, while other nodes repel each other.

For more information, please refer to src/graphcalculator.cpp. (Links will be added soon.)

## Usage

### Requirements & dependencies

This library currently depends on the Qt Framework, specifically some of its libraries. However, I plan to remove 
this dependency soon. For more information, please refer to the issues section.

#### Core dependencies

The core dependencies are necessary to build the main part of this library.

Here are the versions of the tools I used to build this project. Note that these may not be the minimum required versions:

* Build automation tool: **CMake** 3.14.
* **Qt Framework** 5.12
* C++11 compliant compiler. Currently tested on **g++ 9.3.0** and **MSVC 2017**.

#### QML dependencies

If you would like to test or use the QML library for graph representation, you will need the following modules:

* **QtQuick** 2.0
* **QtQuick.Shapes** 1.12
* **QtQuick.Models** 2.12
* **QtQml** 2.0

### How to use in your project

To add this library to your project, you can clone this repository (don't forget `--recurse-submodules`) 
and copy it to your project, or add this repository as a submodule to your project repository.

Then, add `add_subdirectory(graph-layout)` to your `CMakeLists.txt` file, and link your project to the 
`graphlayout` library.

#### Usage of QML library

As an example of graph representation, you can use the QML library that comes with this project. 
To use it in your project, you need to follow the steps above, but you also need to take some additional steps.

Firstly, this library sets the `GRAPH_LAYOUT_QML_RESOURCES` CMake variable. You should add this variable 
to the list of your sources, like this:

```cmake

add_executable(my_very_very_project
	main.cpp
	shit.cpp
	test.cpp
	${GRAPH_LAYOUT_QML_RESOURCES}
)
```

Additionally, you need to add the following line to your project code, where you show your QML:

```cpp

// For example, we're using the QQuickView class

QQuickView view;
view.engine()->addImportPath("qrc:///");	// This line should be added
```

#### Example

You can check out an example project that uses this library at the [graph-layout-gui](https://github.com/Kickoman/graph-layout-gui/) repository.

## Documentation

Documentation coming soon.

# Biblijateka pazycyjanavańnia grafaŭ

Hetaja biblijateka byla stvoranaja dlia testavańnia roznych alharytmaŭ
rysavańnia grafaŭ, a ŭ asnoŭnym -- force-based alharytmaŭ, to-bok tych,
u jakich vykarystoŭvajecca simuliacyja ŭździejańnia roznych silaŭ na
eliementy systemy. Napryklad, u hetaj biblijatecy možna znajści
implementacyju alharytmu "Spružynak i eliektronaŭ" (*ja nia pamiataju
jahony sapraŭdny nazoŭ*). Tak, heta toj alharytam, dzie my ŭjaŭliajem
nibyta rebry grafu jość spružynami, a viaršyni grafu jość eliektronami.
Adpaviedna, zlučanyja viaršyni ciahnucca adna da adnoj, a astatnija --
adšturchoŭvajucca.

Dlia boĺšaj infarmacyi hliadzi `src/graphcalculator.cpp`. (Spasylki ŭžo chutka buduć)

## Vykarystańnie

### Patrabavańni i zaliežnaści

U biahučy momant biblijateka zaliežyć ad Qt, alie ŭ chutkim časie ja
źbirajusia pazbavicca hetych zaližnaściaŭ. Hliadzi ŭ **issues** dlia
boĺšaj infarmacyi.

#### Asnoŭnyja zaliežnaści

Asnoŭnyja zaliežnaści patrebnyja kab sabrać haloŭnuju častku hetaj
biblijateki.

Zaraz tut apisanyja tyja versii pragram, jakija ja sam vykarystoŭvaŭ
dlia zborki hetaha prajektu. Jany mohuć nia być minimaĺna patrebnymi
versijami.

* Systema aŭtamatyzacyi zborki: **Cmake** 3.14
* **Qt Framework** 5.12
* C++ kampiliatar z padtrymkaj c++11. Asabista ja testuju na **g++ 9.3.0** i **MSVC 2017**

#### Zaliežnaści QML

Kali vy zachočacie patestavać biblijateku resursaŭ QML dlia rysavańnia
grafaŭ, vam spatrebiacca hetyja moduli:

* **QtQuick** 2.0
* **QtQuick.Shapes** 1.12
* **QtQuick.Models** 2.12
* **QtQml** 2.0

### Jak vykarystać heta ŭ svaim prajekcie

Zaraz dlia taho, kab dadać hetuju biblijateku ŭ svoj prajekt, vy
majecie toĺki adzin varyjant raźvićcia padziej: klanujcie (`git clone`)
hety repazytor (nie zabudźciesia na `--recurse-submodules`) i pieramiaścicie
jaho ŭ vaš prajekt abo dadajcie hety repazytor jak submoduĺ u repazytor
vašaha prajektu.

Potym prosta dadajcie radok `add_subdirectory(graph-layout)` da vašaha
fajla `CMakeLists.txt` i prylinkujcie vaš prajekt da biblijateki
`graphlayout`.

#### Jak vykarystać biblijateku QML

Dlia prykladnaha adliustravańnia grafu vy možacie vykarystać biblijateku
QML, jakaja pastaŭliajecca z hetym prajektam. Kab zrabić heta vam
spačatku treba vykanać kroki, apisanyja vyšej, a potym -- jašče koĺki dziejańniaŭ.

Perš-napierš, hetaja biblijateka vystaŭliaje pieramiennuju CMake `GRAPH_LAYOUT_QML_RESOURCES`.
Vam treba dadać hetuju pieramiennuju da śpisu fajlaŭ zychodnaha kodu vašaha prajektu jak tut:

```cmake

add_executable(my_very_very_project
    main.cpp
    shit.cpp
    test.cpp
    ${GRAPH_LAYOUT_QML_RESOURCES}
)
```

Taksama vam budzie treba dadać nastupny radok kudyś u svoj kod (niedzie,
dzie vy pakazvajecie vaš QML):

```cpp

// Napryklad, my vykarystoŭvajem QQuickView

QQuickView view;
view.engine()->addImportPath("qrc:///");	// voś hety radok tre' dadać
```

#### Pryklad

Vy možacie aznajomicca z prykladam vykarystańnia hetaj biblijateki ŭ
admyslovym prajekcie pa spasylcy: [graph-layout-gui](https://github.com/Kickoman/graph-layout-gui/).

## Dakumentacyja

Chutka budzie.
