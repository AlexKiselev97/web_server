cmake_minimum_required(VERSION 2.8)                                             
project(final)                                                                  
                                                                                
set(Boost_USE_STATIC_LIBS OFF)                                                  
set(Boost_USE_MULTITHREADED ON)                                                 
set(Boost_USE_STATIC_RUNTIME OFF)                                               
find_package(Boost 1.54.0 COMPONENTS system)                                    
                                                                                
include_directories(${Boost_INCLUDE_DIRS})                                  
                                                                                
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread -lboost_system -fopenmp")                                                                          
                                                                                
set(SRC_LIST main.cpp server.cpp server.h)                                      
                                                                                
add_executable(${PROJECT_NAME} ${SRC_LIST})                                     
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES}) 
