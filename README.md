### Agenda
This library is the implementation of transports for RPi.
The idea behind library is to hide 'boost' library from client code completely.
In all cases 'send' is the call that user does, and receive as truly asynchronous event calls for provided by user callback.

### Compilation
Script ```build.sh``` is building project using necessary GCC. 
You should let it :
```
sudo chmod 777 ${PWD}/build.sh
ls -ls ${PWD}
```
Inside this script, several variables should be re-set to your paths :
```RPI_PTH``` - where everything is placed,
```PRI_TOOLS_PTH``` - path to compiler and sysroot,
```BOOST_ROOT``` - path to the local copy of boost library,
and their dependent paths :
```GDB_PTH```, ```CROSS_COMPILE```, ```CC```, ```CXX```.
Directory ```./build``` must be created manually.
**TODO : Pass variables through ```config.ini```**

Cross compilation is hidden from cmake of the project. All flags are passed inside ```build.sh```.
```build.sh``` calls for ```startDebug.sh``` internally, but this script can be called externally to start GDB server at the RPi side.
Name of the executable (without path) should be passed as sole argument to the ```startDebug.sh``` , example :
``` ./startDebug.sh ./out/TransportTest.out```

Look for RPi tools source here : [raspberry-tools](https://github.com/raspberrypi/tools).

### Submodules :
[Tools](https://github.com/bezumec82/Tools.git)
