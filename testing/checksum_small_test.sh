#!/bin/bash

cd ../server
./serv &
SERV_PID=$!
cd ..
./client 127.0.0.1 testing/small_test_file

kill $SERV_PID

cd testing

sha256sum small_test_file ../server/small_test_file
