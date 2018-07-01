cmake_minimum_required(VERSION 3.9)
project(producer_consumer_semaphore)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS -pthread)

add_executable(ProducerConsumerProcess main.c)