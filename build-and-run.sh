#!/bin/bash

killall denag_server
set -e
g++ -std=c++11 denag_server.cpp -o denag_server
nohup ./denag_server >denag.log 2>&1 &
