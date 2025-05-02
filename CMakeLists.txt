cmake_minimum_required(VERSION 3.31)
project(keepit)

set(CMAKE_CXX_STANDARD 26)

add_executable(keepit main.cpp
        src/filereader/FileReader.cpp
        src/filereader/FileReader.hpp
        src/Controller.cpp
        src/Controller.hpp
        src/wordcounter/WordCounter.cpp
        src/wordcounter/WordCounter.hpp
        src/utils/Logger.hpp
        src/utils/Consts.hpp
        src/threadmanager/ThreadManager.cpp
        src/threadmanager/ThreadManager.hpp
        src/utils/Types.hpp)
