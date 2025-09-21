cmake_minimum_required(VERSION 3.20)
project(OS_Programs)

# Set the C++ standard to a modern version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create an executable target for each program.
# The first argument is the name you'll see in CLion's run menu.
# The second is the source file.
add_executable(CommandSimulator linux_command_simulator.cpp)
add_executable(SchedulingAlgos cpu_scheduling_algorithms.cpp)
add_executable(IPC_Pipes ipc_using_pipes.cpp)
add_executable(ReaderWriter reader_writer_problem.cpp)

# The Reader/Writer program uses pthreads, so we need to find the
# threads library and link it to that specific executable.
find_package(Threads REQUIRED)
target_link_libraries(ReaderWriter PRIVATE Threads::Threads)