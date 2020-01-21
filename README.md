### Compilation
Script 'build.sh' is building project using necessary GCC. 
You should let it :
```
sudo chmod 777 ${PWD}/build.sh
ls -ls ${PWD}
```
### Agenda
This library is the implementation of transports for RPi.
The idea behind library is to hide 'boost' library from client code completely.
In all cases 'send' is the call that user does, and receive as truly asyncronous event calls for callback provided by user.