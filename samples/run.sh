#!/bin/bash
files=`cat run_files.txt`
gcc $files -o a && ./a && rm ./a 