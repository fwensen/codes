#!/bin/bash
g++ ../socketWrap.cpp ../thread.cpp Server.cpp ./ServerSocket.cpp ./OverlayServer.cpp -pthread
