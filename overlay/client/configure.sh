#!/bin/bash
g++ ../socketWrap.cpp ../thread.cpp Server.cpp ./OverlayClient.cpp ./test.cpp -pthread
