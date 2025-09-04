cmake_minimum_required(VERSION 3.9...3.28)

##############################################################################
# Установка компилятора
##############################################################################

  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
      # Настроить Microsoft Visual Studio
      execute_process(COMMAND "C:/Program Files/Microsoft Visual Studio/2022/Enterprise/VC/Auxiliary/Build/vcvars64.bat")
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
      set(COMPILER "-DCMAKE_CXX_COMPILER=g++" CACHE INTERNAL "")
#     set(COMPILER "-DCMAKE_CXX_COMPILER=clang++" CACHE INTERNAL "")
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
      set(COMPILER "-DCMAKE_CXX_COMPILER=clang++" CACHE INTERNAL "")
  endif()