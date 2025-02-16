#!/bin/bash

# 获取脚本所在目录
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
echo "📁 SCRIPT_DIR Directory: '$SCRIPT_DIR' exists."
# 获取项目根目录
PROJECT_ROOT=$(cd "$SCRIPT_DIR/.." && pwd)
echo "📁 PROJECT_ROOT Directory: '$PROJECT_ROOT' exists."
# 指定build目录
BUILD_DIR="$PROJECT_ROOT/build"
echo "📁 BUILD Directory: '$BUILD_DIR' exists."

# 检查build目录是否存在
if [ -d "$BUILD_DIR" ]; then
  echo "📁 Directory '$BUILD_DIR' exists. Proceeding to clear it."

  # 提示确认
  echo "Are you sure you want to clear the '$BUILD_DIR' directory? [y/N] "
  read user_input
    if [[ $user_input =~ ^[Yy]$ ]]; then
    # 清空build目录
    rm -rf "$BUILD_DIR"/*
    echo "🧹 Successfully cleared the '$BUILD_DIR' directory. ✅"
    else
    echo "🛑 Operation cancelled. '$BUILD_DIR' directory was not cleared. ❌"
    fi
else
  echo "📁 Directory '$BUILD_DIR' does not exist. Nothing to clear. ❌"
fi