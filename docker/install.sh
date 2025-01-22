#!/bin/bash

#  Copyright (C) 2018-2025 LEIDOS.
# 
#  Licensed under the Apache License, Version 2.0 (the "License"); you may not
#  use this file except in compliance with the License. You may obtain a copy of
#  the License at
# 
#  http://www.apache.org/licenses/LICENSE-2.0
# 
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
#  License for the specific language governing permissions and limitations under
#  the License.

# Build ros2

cd ~/
rm -rf src/build src/install src/log

source /home/carma/catkin/setup.bash
source /opt/ros/foxy/setup.bash

source /home/carma/.base-image/ros2_msgs_ws/install/setup.bash


colcon build --cmake-args -DCMAKE_BUILD_TYPE=Release --build-base ~/build --install-base /opt/carma/install
