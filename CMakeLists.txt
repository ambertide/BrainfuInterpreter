cmake_minimum_required (VERSION 3.17.0)
project (BRAINFU)

add_executable(brainfu src/main.c src/common.c src/common.h src/compiler.c src/compiler.h src/parser.h src/parser.c)