#!/bin/bash

# 设置变量
IMAGE_NAME="fast_cpp_dev"
CONTAINER_NAME="fast_cpp_dev_container"
HOST_PORT_1=8004    # 代码覆盖率报告端口
HOST_PORT_2=8005      # Nginx 端口
CONTAINER_PORT_1=4444
CONTAINER_PORT_2=5555
HOST_FOLDER="/Users/pengliu/Code/fast_cpp_server"  # 本地文件夹路径
CONTAINER_FOLDER="/workspace"     # 容器内的文件夹路径

# 1. 检查镜像是否存在
echo "检查镜像 ${IMAGE_NAME} 是否存在..."

if [[ "$(docker images -q ${IMAGE_NAME} 2> /dev/null)" == "" ]]; then
    echo "镜像 ${IMAGE_NAME} 不存在，开始构建..."
    
    # 构建 Docker 镜像

    if [ $? -ne 0 ]; then
        echo "构建 Docker 镜像失败"
        exit 1
    fi
else
    echo "镜像 ${IMAGE_NAME} 已存在，跳过构建"
fi

# 2. 打印容器信息
echo "容器 ${CONTAINER_NAME} 已启动，访问以下端口以查看报告和文档："
echo "代码覆盖率报告：http://localhost:${HOST_PORT_1}/coverage_reports"
echo "Doxygen 文档：http://localhost:${HOST_PORT_2}/doxygen_docs"

# 3. 运行 Docker 容器
echo "运行 Docker 容器 ${CONTAINER_NAME}..."

docker run -it \
  --name ${CONTAINER_NAME} \
  -p ${HOST_PORT_1}:${CONTAINER_PORT_1} -p ${HOST_PORT_2}:${CONTAINER_PORT_2} -v ${HOST_FOLDER}:${CONTAINER_FOLDER} \
  ${IMAGE_NAME}:latest /bin/bash 

# 可选：打开容器日志
# docker logs -f ${CONTAINER_NAME}

# docker run -it --name my_container_name -p 8004:4444 -p 8005:5555 -v /code:/workspace fast_cpp_dev:latest /bin/bash
