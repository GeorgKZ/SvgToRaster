#!/bin/bash

cd ../build_scripts
sudo /opt/cmake-4.1.1-linux-x86_64/bin/cmake -DMINGW64=YES -P ./0_setup_qt.cmake
