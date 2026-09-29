# v2x_ros_conversion

The `v2x-ros-conversion` is a ROS2 package currently implemented in [ros2-humble](https://docs.ros.org/en/humble/Installation.html), but previous releases support [ros2-foxy](https://docs.ros.org/en/foxy/Installation.html). It contains two ROS2 nodes, the CPP Message Node and the J2735 Convertor Node, which handle conversion between decoded, structured V2X ROS messages and [carma_driver_msgs/msg/ByteArray](https://github.com/usdot-fhwa-stol/carma-msgs/blob/develop/carma_driver_msgs/msg/ByteArray.msg) messages containing UPER-encoded V2X data for broadcast via an OBU or RSU.

---

## Deployment Instructions

### Deploy using docker (recommended)

1. Pull the latest docker image for conversion from dockerhub

```sh
docker pull usdotfhwastoldev/v2x-ros-conversion:develop # or release of your choice
```

#### Note: Versions up to carma-system-4.5.0 (ros2-foxy supported) can all be found under [usdotfhwastoldev/carma-cohda-dsrc-driver<release-tag>](https://hub.docker.com/r/usdotfhwastoldev/carma-cohda-dsrc-driver)

2. Run the Docker image

```sh
docker run -it --network host usdotfhwastoldev/v2x-ros-conversion:develop
```

---

### Build from Source

#### Note: Assumption here is that user is building on a ros2 humble development environment

1. Create the workspace and source directory, or enter it if it already exsists.

```sh
mkdir v2x_ws/src
# and or
cd v2x_ws/src
```

2. Clone the repository into workspace.

```sh
git clone https://github.com/usdot-fhwa-stol/v2x-ros-conversion.git
```

3. Clone the dependencies into the workspace

```sh
chmod +x v2x-ros-conversion/docker/checkout.bash
./v2x-ros-conversion/docker/checkout.bash -r v2x-ros-conversion/ -b develop # or release tag / branch of your choice
```

4. Build the package.

```sh
source /opt/ros/humble/setup.bash
colcon build --packages-up-to v2x-ros-conversion
```

5. Launch the node.

```sh
cd .. # back into your workspace root
source install/setup.bash
ros2 launch v2x-ros-conversion v2x-ros-conversion.launch.py
```
