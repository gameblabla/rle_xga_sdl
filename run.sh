#!/bin/sh
./main.elf eye.png eye.dat > ./log_1.txt
./dec.elf eye.dat eye.png > ./log_2.txt
