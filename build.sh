#!/bin/bash


# 创建build目录
if [ -d "build" ]; then
  echo "📁 Directory 'build' already exists. ✅"
elif mkdir build; then
  echo "📁 Successfully created build directory. ✅"
else
  echo "📁 Failed to create build directory. ❌"
fi

# 创建external目录
if [ -d "external" ]; then
  echo "📁 Directory 'external' already exists. ✅"
elif mkdir external; then
  echo "📁 Successfully created external directory. ✅"
else
  echo "📁 Failed to create external directory. ❌"
fi

# 创建include目录
if [ -d "include" ]; then
  echo "📁 Directory 'include' already exists. ✅"
elif mkdir include; then
  echo "📁 Successfully created include directory. ✅"
else
  echo "📁 Failed to create include directory. ❌"
fi

# 创建src目录
if [ -d "src" ]; then
  echo "📁 Directory 'src' already exists. ✅"
elif mkdir src; then
  echo "📁 Successfully created src directory. ✅"
else
  echo "📁 Failed to create src directory. ❌"
fi

# 创建test目录
if [ -d "test" ]; then
  echo "📁 Directory 'test' already exists. ✅"
elif mkdir test; then
  echo "📁 Successfully created test directory. ✅"
else
  echo "📁 Failed to create test directory. ❌"
fi

# 创建tools目录
if [ -d "tools" ]; then
  echo "📁 Directory 'tools' already exists. ✅"
elif mkdir tools; then
  echo "📁 Successfully created tools directory. ✅"
else
  echo "📁 Failed to create tools directory. ❌"
fi

# 创建util目录
if [ -d "util" ]; then
  echo "📁 Directory 'util' already exists. ✅"
elif mkdir util; then
  echo "📁 Successfully created util directory. ✅"
else
  echo "📁 Failed to create util directory. ❌"
fi

# 创建scripts目录
if [ -d "scripts" ]; then
  echo "📁 Directory 'scripts' already exists. ✅"
elif mkdir scripts; then
  echo "📁 Successfully created scripts directory. ✅"
else
  echo "📁 Failed to create scripts directory. ❌"
fi

# 创建scripts目录
if [ -d "docs" ]; then
  echo "📁 Directory 'docs' already exists. ✅"
elif mkdir docs; then
  echo "📁 Successfully created docs directory. ✅"
else
  echo "📁 Failed to create docs directory. ❌"
fi

# 创建CMakeLists.txt文件
if [ -f "CMakeLists.txt" ]; then
  echo "📄 File 'CMakeLists.txt' already exists. ✅"
elif touch CMakeLists.txt; then
  echo "📄 Successfully created CMakeLists.txt file. ✅"
else
  echo "📄 Failed to create CMakeLists.txt file. ❌"
fi

# 下载spdlog
if [ -d "external/spdlog" ]; then
  echo "⬇️ Directory 'external/spdlog' already exists. ✅"
elif git clone https://github.com/gabime/spdlog.git external/spdlog; then
  echo "⬇️ Successfully downloaded spdlog. ✅"
else
  echo "⬇️ Failed to download spdlog. ❌"
fi

# 下载Google Test
if [ -d "external/googletest" ]; then
  echo "⬇️ Directory 'external/googletest' already exists. ✅"
elif git clone https://github.com/google/googletest.git external/googletest; then
  echo "⬇️ Successfully downloaded Google Test. ✅"
else
  echo "⬇️ Failed to download Google Test. ❌"
fi


# 下载jsoncpp
if [ -d "external/jsoncpp" ]; then
  echo "⬇️ Directory 'external/jsoncpp' already exists. ✅"
elif git clone https://github.com/open-source-parsers/jsoncpp.git external/jsoncpp; then
  echo "⬇️ Successfully downloaded JSONCPP. ✅"
else
  echo "⬇️ Failed to download JSONCPP. ❌"
fi

# 下载nlohmann/json
if [ -d "external/json" ]; then
  echo "⬇️ Directory 'external/json' already exists. ✅"
elif git clone https://github.com/nlohmann/json.git external/json; then
  echo "⬇️ Successfully downloaded nlohmann/json. ✅"
else
  echo "⬇️ Failed to download nlohmann/json. ❌"
fi

# 下载nlohmann/cpr
if [ -d "external/cpr" ]; then
  echo "⬇️ Directory 'external/cpr' already exists. ✅"
elif git clone https://github.com/libcpr/cpr.git external/cpr; then
  echo "⬇️ Successfully downloaded libcpr/cpr. ✅"
else
  echo "⬇️ Failed to download libcpr/cpr. ❌"
fi

# create main.cpp
if [ -f "src/main.cpp" ]; then
  echo "📄 File 'src/main.cpp' already exists. ✅"
elif touch src/main.cpp; then
  echo "📄 Successfully created main.cpp file. ✅"
else
  echo "📄 Failed to create main.cpp file. ❌"
fi

# create build_google_test_framework.sh
if [ -f "scripts/build_google_test_framework.sh" ]; then
  echo "📄 File 'scripts/build_google_test_framework.sh' already exists. ✅"
elif touch scripts/build_google_test_framework.sh; then
  echo "📄 Successfully created build_google_test_framework.sh file. ✅"
else
  echo "📄 Failed to create build_google_test_framework.sh file. ❌"
fi

# create build_spdlog_lib.sh
if [ -f "scripts/build_spdlog_lib.sh" ]; then
  echo "📄 File 'scripts/build_spdlog_lib.sh' already exists. ✅"
elif touch scripts/build_spdlog_lib.sh; then
  echo "📄 Successfully created build_spdlog_lib.sh file. ✅"
else
  echo "📄 Failed to create build_spdlog_lib.sh file. ❌"
fi

# create clear_build_dir.sh
if [ -f "scripts/clear_build_dir.sh" ]; then
  echo "📄 File 'scripts/clear_build_dir.sh' already exists. ✅"
elif touch scripts/clear_build_dir.sh; then
  echo "📄 Successfully created clear_build_dir.sh file. ✅"
else
  echo "📄 Failed to create clear_build_dir.sh file. ❌"
fi