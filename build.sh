#!/bin/sh
build_mode="debug"

files="main.cpp glad.c"
link_opts="-lglfw3 -lwayland-client"
output_name=main
include_path=include
lib_path=lib

build_command="g++ -g -Og $files $link_opts -o $output_name -I $include_path -L $lib_path"

# exec
# build AND run
$build_command
# run=1
# ./$output_name
