cmake_minimum_required(VERSION 3.20)
project(interview_card_reader_c C)

set(CMAKE_C_STANDARD 99)

add_executable(interview_card_reader_c logChecker.c logChecker.h test.c)
