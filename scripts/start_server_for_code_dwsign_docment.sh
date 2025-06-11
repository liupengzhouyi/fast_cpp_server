#!/bin/bash

# 获取脚本所在目录
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
echo "📁 SCRIPT_DIR Directory: '$SCRIPT_DIR' exists."
# 获取项目根目录
PROJECT_ROOT=$(cd "$SCRIPT_DIR/.." && pwd)
echo "📁 PROJECT_ROOT Directory: '$PROJECT_ROOT' exists."
# 指定 DOC_HTML 目录
DOC_HTML_DIR="$PROJECT_ROOT/docs/html"
echo "📁 DOC_HTML_DIR Directory: '$DOC_HTML_DIR' exists."

# 检查build目录是否存在
if [ -d "$DOC_HTML_DIR" ]; then
  echo "📁 Directory '$DOC_HTML_DIR' exists. Proceeding to clear it."
  cd $DOC_HTML_DIR && python3 -m http.server 5555
else
  echo "📁 Directory '$DOC_HTML_DIR' does not exist. Nothing to clear. ❌"
fi