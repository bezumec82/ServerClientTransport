reset
export NORM="\e[39m"    ; #echo -e "${NORM}Normal output";
export RED="\e[31m"     ; #echo -e "${RED}Red output";
export GRN="\e[32m"     ; #echo -e "${GRN}Green output";
export CYN="\e[36m"     ; #echo -e "${CYN}Cyan output";
export MAG="\e[35m"     ; #echo -e "${MAG}Magenta output";
export YEL="\e[33m"     ; #echo -e "${YEL}Yellow output";
export BLU="\e[34m"     ; #echo -e "${BLU}Blue output";

echo -e "${RED} !!! All pathes should be setup manually !!!${NORM}"
export RPI_PTH=/media/constantine/Work/BOARDS/RASPBERRY
export PRI_TOOLS_PTH=${RPI_PTH}/tools-master
export BOOST_ROOT=/media/constantine/Work/LIBRARIES/boost_1_71_0

export GDB_PTH=${PRI_TOOLS_PTH}/arm-bcm2708/arm-linux-gnueabihf/bin
export CROSS_COMPILE=${GDB_PTH}/arm-linux-gnueabihf-
export CC=${CROSS_COMPILE}gcc
export CXX=${CROSS_COMPILE}g++

export PROJECT_PTH=${PWD}

if [ ! -f ${CC} ]; then
echo -e "${RED} GCC compiler is not found at : ${CC}${NORM}"
exit 1
fi
if [ ! -f ${CXX} ]; then
echo -e "${RED} GCC C++ compiler is not found at : ${CXX}${NORM}"
exit 1
fi
if [ ! -d ${BOOST_ROOT} ]; then
echo -e "${RED} Boost is not found at : ${BOOST_ROOT}${NORM}"
exit 1
fi


echo -e "${CYN}Project is located at : ${PROJECT_PTH}${NORM}"

if [ -d "${PROJECT_PTH}/build" ]; then
    echo -e "${YEL}Clearing 'build' directory${NORM}"
    rm -rf ${PROJECT_PTH}/build/*
else
    echo -e "${RED}Creating 'build' directory${NORM}"
    mkdir ${PROJECT_PTH}/build
fi

if [ -d "${PROJECT_PTH}/out" ]; then
    echo -e "${YEL}Clearing 'out' directory${NORM}"
    rm -rf ${PROJECT_PTH}/out/*
else
    echo -e "${RED}Creating 'out' directory${NORM}"
    mkdir ${PROJECT_PTH}/out
fi

#########
### BUILD
#########
cd ${PROJECT_PTH}/build
echo -e "${MAG}Running CMake in dir : ${PWD}${NORM}"
cmake \
-D CMAKE_C_COMPILER=${CC} \
-D CMAKE_C_FLAGS="${CMAKE_CXX_FLAGS} -g -pthread -mfpu=neon -DLINUX_TERMINAL" \
-D CMAKE_C_COMPILER_WORKS=1 \
-D CMAKE_CXX_COMPILER=${CXX} \
-D CMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -g -pthread -std=c++14 -mfpu=neon -DLINUX_TERMINAL" \
-D CMAKE_CXX_COMPILER_WORKS=1 \
-D CMAKE_BUILD_TYPE=Debug \
\
-D BOOSTROOT=${BOOST_ROOT} \
-D Boost_INCLUDE_DIR=${BOOST_ROOT} \
\
-D CMAKE_SYSROOT=${PRI_TOOLS_PTH}/arm-bcm2708/arm-linux-gnueabihf/arm-linux-gnueabihf/sysroot \
-D CMAKE_FIND_ROOT_PATH=${PRI_TOOLS_PTH}/arm-bcm2708/arm-linux-gnueabihf/arm-linux-gnueabihf \
-D CMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
-D CMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
-D CMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
-D CMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY \
-D CMAKE_LIBRARY_PATH=${CMAKE_SYSROOT}/usr/lib64 \
-D CROSS_COMPILE_INCLUDES=${CMAKE_SYSROOT}/usr/include/linux \
\
-D CMAKE_INSTALL_PREFIX=${PROJECT_PTH}/out \
-D CMAKE_RUNTIME_OUTPUT_DIRECTORY=${PROJECT_PTH}/out \
-D CMAKE_LIBRARY_OUTPUT_DIRECTORY=${PROJECT_PTH}/out \
-D CMAKE_ARCHIVE_OUTPUT_DIRECTORY=${PROJECT_PTH}/out \
../

echo -e "${CYN}Running make${NORM}"
make DESTDIR=${PROJECT_PTH}/out

#########
### DEBUG
#########
export RMT_IP=192.168.1.72
export RMT_USR=pi
export RMT_HOST_PASS=pi
export RMT_HOST=${RMT_USR}@${RMT_IP}
export TGT_EXE_PTH=/home/pi/remote-debug
export GDB_PORT=2345
# One at a time !
export EXECTBL=UnixSocketTransportTest.out

if ls -1qA "${PROJECT_PTH}/out" | grep -q .
then
echo -e "${GRN}Build success. Copying ${EXECTBL} to the Raspberry : ${TGT_EXE_PTH}.${NORM}"
sshpass -p "${RMT_HOST_PASS}" \
scp ${PROJECT_PTH}/out/${EXECTBL} ${RMT_HOST}:${TGT_EXE_PTH}/${EXECTBL}

echo -e "${GRN}Starting GDB server with port ${GDB_PORT}${NORM}"
sshpass -p "${RMT_HOST_PASS}" ssh -tt ${RMT_HOST} << EOF
killall -9 gdbserver && killall -9 gdbserver
gdbserver :${GDB_PORT} ${TGT_EXE_PTH}/${EXECTBL}
EOF
else
    echo -e "${RED}Build failure${NORM}"
fi