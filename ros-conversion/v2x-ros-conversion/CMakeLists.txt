
# Copyright (C) 2024 LEIDOS.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

cmake_minimum_required(VERSION 3.5)
project(v2x-ros-conversion)

find_package(carma_cmake_common REQUIRED)
carma_package()

carma_check_ros_version(2)

list(APPEND CMAKE_PREFIX_PATH "/opt/carma/cmake/")
set(CMAKE_INSTALL_RPATH "/opt/carma/lib")


find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

ament_auto_package(
        INSTALL_TO_SHARE launch log-config scripts
)
