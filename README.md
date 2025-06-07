# fast_cpp_server

A template of cpp server.


### Add tools for cpp porject

* code coverage
* code doc genarte tools


## 

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

## FQA

* if MQTT con't start, run:
```shell
export LD_LIBRARY_PATH=/workspace/build/external/mosquitto/lib:$LD_LIBRARY_PATH
```