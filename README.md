# TerraLib GeoWeb Services

**TerraLib GeoWeb Services (TWS)** is a free and open source platform for the development of web services that must handle Earth Observation data. TWS is part of the [TerraLib family of geotechnologies](http://www.dpi.inpe.br/terralib5).

The following service is under development/investigation:
- **Web Time Series Service (WTSS):** a lightweight web service for handling time series data from remote sensing imagery.
- **Web Coverage Service 2.0 (WCS):** a web coverage service for data exchange with SciDB.
- **Spatio-Temporal Web Map Service (WMS):** a web map service for visualizing remote sensing imagery data as dynamic maps (space + time) stored as 3D arrays in SciDB.

**NOTE:**
* **Until we reach version 1.0.0 this codebase will be instable and not fully operational.**
* **TWS is under active development. We are preparing this site to host it!**
* **If you have any question, please, send us an e-mail at: esensing-team@dpi.inpe.br.**

## Source Code Organization

### Codebase Documentation
In the root directory of TWS codebase (the source code tree) there are some text files explaining the details of the codebase:

- **[BRANCHES-AND-TAGS:](https://github.com/e-sensing/tws/blob/master/BRANCHES-AND-TAGS)** Notes on how to switch to the right branch to work on or the right tag to get the source code.

- **[BUILD-INSTRUCTIONS:](https://github.com/e-sensing/tws/blob/master/BUILD-INSTRUCTIONS)** Notes on how to compile and install TWS for each platform.

- **CHANGELOG:** List of changes in TWS source code. **Not available yet!**

- **[DEPENDENCIES:](https://github.com/e-sensing/tws/blob/master/DEPENDENCIES)** The list of third-party library you must install before building TWS.

- **[LICENSE:](https://github.com/e-sensing/tws/blob/master/LICENSE)** Licence statement in plain txt format.

- **[README:](https://github.com/e-sensing/tws/blob/master/README)** Contains instructions about how to build and how is organized TWS plataform source code.

### Directories

- **[build/cmake:](https://github.com/e-sensing/tws/tree/master/build/cmake)** Contains the CMake scripts with commands, macros and functions used to build the environment for compiling libraries and executables in different platforms using CMake.

- **[examples:](https://github.com/e-sensing/tws/tree/master/examples)** Some examples on how to use TWS API and Services.
 
- **[install:](https://github.com/e-sensing/tws/tree/master/install)** Bash scripts for helping building and installing TWS.

- **licenses:** Copyright notices of third-party libraries used by TWS. **Not available yet!**

- **[resources:](https://github.com/e-sensing/tws/tree/master/resources)** Fonts, images, sql, and xml files among other resources of general use.

- **[share:](https://github.com/e-sensing/tws/tree/master/share)** XML Schema (.xsd), JSON files, plugin manifest files, translations files and OGC specifications that is shared and installed with TWS.

- **[src:](https://github.com/e-sensing/tws/tree/master/src)** Contains the source code of TWS and its automatic test system.
 
## Dependencies

The file named **[DEPENDENCIES](https://github.com/e-sensing/tws/blob/master/DEPENDENCIES)** in the root of TWS source tree contains the official list of third-party libraries and tools that you must install before building TWS from source.

If you want to build yourself TWS then you need to install some third-party libraries. Below we show the list of third-party libraries dependencies and its versions:
- **Boost (Mandatory):** TWS is built on top of Boost libraries. You will need to have them installed in order to build TWS. Make sure to have at least version 1.54.0 installed. If you prefer to install from source, download it from: http://www.boost.org.

- **TerraLib (Mandatory):** TWS is built on top of TerraLib, a free and open source library for building GIS enabled applications. Make sure to have at least TerraLib version 5.1.0. You can download it from: http://www.dpi.inpe.br/terralib5.

**Note:**
- When using TWS on a machine with a installed version of SciDB you probably already have Boost libraries installed.
- If you have installed TerraLib you already have Boost and you don't need to install it again.


### Bash script for building all dependencies on Linux Ubuntu 14.04

We have prepared a special bash script for building and installing the dependencies on Linux Ubuntu 14.04. This script can be found in TWS source tree under *install* folder. Follow the steps below:

- Download the third-party libraries package used by the development team: [tws-3rdparty-linux-ubuntu-14.04.tar.gz](http://www.dpi.inpe.br/esensing-devel/tws-3rdparty-linux-ubuntu-14.04.tar.gz).

- Copy the script [install-3rdparty-linux-ubuntu-14.04.sh](https://github.com/e-sensing/tws/tree/master/install/install-3rdparty-linux-ubuntu-14.04.sh) to the same folder you have downloaded the *tws-3rdparty-linux-ubuntu-14.04.tar.gz* package.

- Open the shell command line and call the script *install-3rdparty-linux-ubuntu-14.04.sh* setting the target to install all the stuffs from these third-party libraries and tools:
```
$ TWS_DEPENDENCIES_DIR="/home/user/mylibs" ./install-3rdparty-linux-ubuntu-14.04.sh
```

**Note:* Don't choose as target location, a system folder such as /usr or /usr/local. Try some user specifiic folder.

### Bash script for building all dependencies on Mac OS X El Capitan

We have prepared a special bash script for building and installing the dependencies on Mac OS X El Capitan. This script can be found in TWS source tree under *install* folder. Follow the steps below:

- Download the third-party libraries package used by the development team: [tws-3rdparty-macosx-el-capitan.tar.gz](http://www.dpi.inpe.br/esensing-devel/tws-3rdparty-macosx-el-capitan.tar.gz).

- Copy the script [install-3rdparty-macosx-el-capitan.sh](https://github.com/e-sensing/tws/tree/master/install/install-3rdparty-macosx-el-capitan.sh) to the same folder you have downloaded the *tws-3rdparty-macosx-el-capitan.tar.gz* package.

- Open the shell command line.

- Make sure your CMake environment can be found in your PATH:
```
$ export PATH=$PATH:/Applications/CMake.app/Contents/bin
```

- In the shell command line, call the script *install-3rdparty-macosx-el-capitan.sh* setting the target to install all the stuffs from these third-party libraries and tools:
```
$ TWS_DEPENDENCIES_DIR="/Users/user/mylibs" ./install-3rdparty-macosx-el-capitan.sh
```

**Note:** Don't choose as target location, a system folder such as */usr* or */usr/local*. Try some user specific folder.

### Prepared dependencies for Microsot Windows

**THIS SECTION IS UNDER DEVELOPMENT**


## Cloning TWS Repository

- Open the shell command line.

- Make a new folder to host TWS source code:
```
$ mkdir -p /home/user/mydevel/tws/codebase
```

- Change the current directory to that new folder:
```
$ cd /home/user/mydevel/tws/codebase
```

- Make a local copy of TWS repository:
```
$ git clone https://github.com/e-sensing/tws.git .
```

## Branches
You can check all branches available (remotes and local) and see the current one (marked with "*"):

`$ git branch -a`

The output of above command will be something like:
```
  * master
  remotes/origin/HEAD -> origin/master
  remotes/origin/master
```

In the above output the "* master" means that the current branch is master.

We have the following branches:
- **master:** This is the branch where the development team is working to add new features to future versions of TWS. It may be instable although the codebase is subject to automatic tests (regression and unittests). We don't recommend to generate production versions of TWS from this branch. Use it for testing new features and get involved with TWS development.

- **b-1.0.0-alpha:** This will be the first branch in TWS' codebase for the generation 1.

To switch to one of the branches listed above, use the checkout command and create a local branch to track the remote branch. The syntax of "git checkout" is:

`$ git checkout -b <local_branch_name> <remote_branch_name without this part "remotes/">`

In order to switch to branch *b-1.0.0-alpha* you can use the following command:

`$ git checkout -b b-1.0.0-alpha origin/b-1.0.0-alpha`

## Tags

Also there are tags which usually are originated from a release branch. For instance, tag *t-1.0.0-alpha1* will be originated from branch *b-1.0.0-alpha*.

To check all tags available, use:

`$ git tag -l           (list all tag names)`
```
  t-1.0.0-alpha1
  t-1.0.0-alpha2
  t-1.0.0-beta1
  t-1.0.0-rc1
  t-1.0.0
  ...
```

If you want to checkout a specific version given by a tag and create a local branch to work on you can use the following git command:

`$ git checkout -b <local_branch_tag_name> <one_of_tag_name_listed>`

For instance, to checkout *t-1.0.0-alpha1* you can enter the following command:

`$ git checkout -b t-1.0.0-alpha1  t-1.0.0-alpha1`

## Build Instructions

After choosing the right branch or tag to work on, follow the insructions on **DEPENDENCIES** section. Make sure you have all the third-party library dependencies listed in this section before trying to build TWS.

The `build/cmake` folder contains a CMake project for building TWS.

Until now its build has been tested on:
- Linux Ubuntu 14.04
- Mac OS X El Capitan

You should use at least CMake version 2.8.12 for building TWS. Older versions than this may not work properly.

Follow the build steps below according to your platform.

### Building on Linux with GNU G++

1.1. Open a Command Prompt (Shell).

1.2. We will assume that the codebase (all the source tree) is located at:

`/home/user/mydevel/tws/codebase`

1.3. Create a folder out of the TWS source tree to generate the build system, for example:
```
$ cd /home/user/mydevel/tws
$ mkdir build-release
$ cd build-release
```
**Note:** for the sake of simplicity create this directory in the same level as the source tree (as showned above).

1.4. For Linux systems you must choose the build configuration:
```
$ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING="Release" -DCMAKE_INSTALL_PREFIX:PATH="/home/user/myinstall/tws" -DCMAKE_PREFIX_PATH:PATH="/home/user/mylibs;/home/user/mylibs/terralib5/lib/cmake" ../codebase/build/cmake
```

1.5 Building (with 4 process in parallel):
```
$ make -j 4
```

1.6 Installing:
```
$ make install
```

1.7 Uninstalling:
```
$ make uninstall
```

Notes:
* Some Linux flavours with different versions of GNU gcc and Boost will need more parameters such as:
```
  -DCMAKE_INCLUDE_PATH:PATH="/usr/local;/opt/include"
  -DCMAKE_LIBRARY_PATH:PATH="/usr/local;/opt/lib"
  -DCMAKE_PROGRAM_PATH:PATH="/usr/local/bin;/opt/bin"
  -DBOOST_ROOT:PATH="/opt/boost"
```
* Boost can also be indicated by BOOST_INCLUDEDIR (note: without an '_' separating INCLUDE and DIR):
```
  -DBOOST_INCLUDEDIR:PATH="/usr/local/include"
```
* The parameter -lpthread must be informed only if your Boost was not built as a shared library:
```
  -DCMAKE_CXX_FLAGS:STRING="-lpthread"
```
* For generating a debug version set CMAKE_BUILD_TYPE as:
```
  -DCMAKE_BUILD_TYPE:STRING="Debug"
```

### Building on Mac OS X El Capitan

1.1 Open a Command Prompt (Shell).

1.2. We will assume that the codebase (all the source tree) is located at:
```
/Users/user/mydevel/tws/codebase
```

1.3. Create a folder out of the TWS source tree to generate the build system, for example:
```
$ cd /Users/user/mydevel/tws
$ mkdir build-release
$ cd build-release
```
**Note:** for the sake of simplicity create this directory in the same level as the source tree (as showned above).

1.4. For Mac OS X systems you must choose the build configuration:
```
$ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING="Release" -DCMAKE_INSTALL_PREFIX:PATH="/Users/user/myinstall/tws" -DCMAKE_PREFIX_PATH:PATH="/Users/user/mylibs;/Users/user/mylibs/terralib5/lib/cmake" ../codebase/build/cmake
```

**Note:** Please, in the cmake call above, take special attention to the key *CMAKE_PREFIX_PATH* and TerraLib location.

1.5. Building (with 4 process in parallel):
```
$ make -j 4
```

1.6. Installing:
```
$ make install
```
1.7. Uninstalling:
```
$ make uninstall
```

Notes:

* You have also to tell where the TerraLib CMake support is located. Add to CMAKE_PREFIX_PATH where TerraLib is installed, for example:
```
/Users/user/MyLibs/terralib/lib/cmake
```

* You can also generate an Xcode project by using the "Xcode generator" option:
```
-G "Xcode"
```

* There are some useful variables that can be set inside Xcode in order to run an application. The following environment variable can be set:
```
DYLD_FALLBACK_LIBRARY_PATH
DYLD_FALLBACK_FRAMEWORK_PATH
```

### Building on Microsoft Windows with Visual C++

**TO BE DONE**


### Quick Notes for Developers

If you have built TWS in Debug mode and you want to run it inside the build tree, you may need to set some environment variables.

For Mac OS X, you can set the following variables:
```
$ export DYLD_FALLBACK_LIBRARY_PATH=/Users/user/MyLibs/lib
$ export DYLD_FALLBACK_FRAMEWORK_PATH=/Users/user/MyLibs/lib/
```

## Reporting Bugs

Any problem should be reported to esensing-team@dpi.inpe.br.


For more information on TWS, please, visit its main web page at: http://www.dpi.inpe.br/esensing.
