# fast_cpp_server

A template of cpp server.


### Add tools for cpp porject

* code coverage
* code doc genarte tools
* tcp relink client

## docker dev

### gen container

```bash
docker run -it --name my_container_name -p 8004:4444 -p 8005:5555 -v /code:/workspace fast_cpp_dev:latest /bin/bash
```

## show code coverage in docker server

![file_tree](./images/coverage.png)

### show code document

![file_tree](./images/file_tree.png)

![file_tree](./images/class_info.png)


## How can I compile binary executable file ?

```shell
cd ${PROJECT_ROOT}
mkdir build
cd ./build
cmake ..
make -j
```

## How can I Exec unit test ?

```shell
cd ${PROJECT_ROOT}
mkdir build
cd ./build
cmake ..
make -j
# ------------
cd ./bin
ls -l | grep "fast"
# -rwxr-xr-x 1 root root  2496096 Jun 11 18:47 fast_cpp_server
# -rwxr-xr-x 1 root root  2299192 Jun 11 18:48 fast_cpp_server_Test
cd -

ctest 

```

## How can I generate Code coverage report ?

```shell
cd ${PROJECT_ROOT}
cd build
# compile binary executable file
#  Exec unit test
make coverage

cd ./../scripts/
sh gen_coverage_report.sh

```

## How can I generate Code Design document ?

```shell
cd ${PROJECT_ROOT}
doxygen Doxyfile
sh start_server_for_code_dwsign_docment.sh

```

## FQA

* if MQTT con't start, run:
```shell
export LD_LIBRARY_PATH=/workspace/build/external/mosquitto/lib:$LD_LIBRARY_PATH
```