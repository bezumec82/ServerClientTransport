### Agenda
This library is the implementation of transports for RPi.
The idea behind library is to hide 'boost' library from client code completely.
In all cases 'send' is the call that user does, and receive as truly asyncronous event calls for callback provided by user.

### Compilation
Script 'build.sh' is building project using necessary GCC. 
You should let it :
```
sudo chmod 777 ${PWD}/build.sh
ls -ls ${PWD}
```
Cross compilation is hidden from cmake of the project. All flags are passed inside 'build.sh'.
'build.sh' calls for 'startDebug.sh' internally, but this script can be called externally to start GDB server at the RPi side.
Name of the executable (without path) should be passed as sole argument to the 'startDebug.sh'.

Inside 'build.sh', there are variables that provides path to some necessary facilities :

RPI_PTH - where everething is places.

PRI_TOOLS_PTH - path to compiler and sysroot.

Look here for source here : [raspberry-tools](https://github.com/raspberrypi/tools).
BOOST_ROOT - path to the boost library. I've avoided of using of compilable part.