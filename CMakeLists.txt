cmake_minimum_required(VERSION 3.14)
project(sx128x_linux_driver)

set(CMAKE_CXX_STANDARD 17)

set(CPM_DOWNLOAD_VERSION 0.27.2)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
        NAME sx128x_driver
        GITHUB_REPOSITORY pete4radio/sx128x_driver
	VERSION 1.0.1
)

CPMAddPackage(
        NAME GPIOPlusPlus
        GITHUB_REPOSITORY YukiWorkshop/GPIOPlusPlus
        VERSION 0.0.2
)

CPMAddPackage(
        NAME SPPI
        GITHUB_REPOSITORY YukiWorkshop/SPPI
        VERSION 0.0.5
)

CPMAddPackage(
        NAME JSON
        GITHUB_REPOSITORY nlohmann/json
        VERSION 3.11.3
)

add_library(sx128x_linux_driver SX128x_Linux.cpp SX128x_Linux.hpp pins.hpp)
target_include_directories(sx128x_linux_driver PUBLIC 
${sx128x_driver_SOURCE_DIR} 
${GPIOPlusPlus_SOURCE_DIR} 
${SPPI_SOURCE_DIR} 
${JSON_SOURCE_DIR} 
/home/pi/Documents/github/2427_radio/build/_deps/json-src/single_include/nlohmann
)
target_link_libraries(sx128x_linux_driver pthread GPIOPlusPlus SPPI sx128x_driver)

# Build tx file
add_executable(Lora_tx Lora_tx.cpp)
target_link_libraries(Lora_tx sx128x_linux_driver)

# Build packet tx file
add_executable(Lora_tx_packets Lora_tx_packets.cpp)
target_link_libraries(Lora_tx_packets sx128x_linux_driver)

# Build rx file
add_executable(Lora_rx Lora_rx.cpp)
target_link_libraries(Lora_rx sx128x_linux_driver)

# Build test file
add_executable(Test Test.cpp)
target_link_libraries(Test sx128x_linux_driver)

# Build link test, which sends packets at different powers and records the histogram of received packets
add_executable(LinkTest LinkTest.cpp)
target_link_libraries(LinkTest sx128x_linux_driver)

# Build the original test file too
add_executable(Old_Test LinkTest_Proto.cpp)
target_link_libraries(Old_Test sx128x_linux_driver)

# Build the unit tests
add_executable(Chip_ID Chip_ID.cpp)
target_link_libraries(Chip_ID sx128x_linux_driver)

#Build the test executive "runner"
add_executable(runner runner.cpp)
target_link_libraries (runner)

# Build link test, which sends packets at different powers and records the histogram of received packets.
#  It gets the modulation parameters from a json file
add_executable(LinkTestJ LinkTestJ.cpp)
target_link_libraries(LinkTestJ sx128x_linux_driver nlohmann_json::nlohmann_json)