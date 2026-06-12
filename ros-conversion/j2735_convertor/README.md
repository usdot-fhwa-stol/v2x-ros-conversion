# j2735_convertor

The `j2735_convertor` is a ROS2 package providing bidirectional conversion between standard J2735/J3224 V2X message types and their internal ROS2 representations, supporting BSM, PSM, SPaT, MAP, SDSM, and Traffic Control messages.

## ROS Topics

### Published

* `/incoming_bsm` : `carma_v2x_msgs/msg/BSM` — BSM converted from J2735 format, published for use by the rest of the system.
* `/outgoing_j2735_bsm` : `j2735_v2x_msgs/msg/BSM` — BSM converted to J2735 format, published for UPER encoding by cpp_message.
* `/incoming_psm` : `carma_v2x_msgs/msg/PSM` — PSM converted from J2735 format, published for use by the rest of the system.
* `/outgoing_j2735_psm` : `j2735_v2x_msgs/msg/PSM` — PSM converted to J2735 format, published for UPER encoding by cpp_message.
* `/incoming_spat` : `carma_v2x_msgs/msg/SPAT` — SPaT message converted from J2735 format, published for use by the rest of the system.
* `/incoming_map` : `carma_v2x_msgs/msg/MapData` — MapData message converted from J2735 format, published for use by the rest of the system.
* `/incoming_sdsm` : `carma_v2x_msgs/msg/SensorDataSharingMessage` — SDSM converted from J3224 format, published for use by the rest of the system.
* `/outgoing_j3224_sdsm` : `j3224_v2x_msgs/msg/SensorDataSharingMessage` — SDSM converted to J3224 format, published for UPER encoding by cpp_message.
* `/incoming_geofence_control` : `carma_v2x_msgs/msg/TrafficControlMessage` — Traffic control message converted from J2735 format, published for use by the rest of the system.
* `/incoming_geofence_request` : `carma_v2x_msgs/msg/TrafficControlRequest` — Traffic control request converted from J2735 format, published for use by the rest of the system.
* `/outgoing_j2735_geofence_control` : `j2735_v2x_msgs/msg/TrafficControlMessage` — Traffic control message converted to J2735 format, published for UPER encoding by cpp_message.
* `/outgoing_j2735_geofence_request` : `j2735_v2x_msgs/msg/TrafficControlRequest` — Traffic control request converted to J2735 format, published for UPER encoding by cpp_message.

### Subscribed

* `/incoming_j2735_bsm` : `j2735_v2x_msgs/msg/BSM` — J2735 BSM decoded and published by cpp_message, to be converted to internal format.
* `/outgoing_bsm` : `carma_v2x_msgs/msg/BSM` — Internal BSM to be converted to J2735 format for outbound transmission.
* `/incoming_j2735_psm` : `j2735_v2x_msgs/msg/PSM` — J2735 PSM decoded and published by cpp_message, to be converted to internal format.
* `/outgoing_psm` : `carma_v2x_msgs/msg/PSM` — Internal PSM to be converted to J2735 format for outbound transmission.
* `/incoming_j2735_spat` : `j2735_v2x_msgs/msg/SPAT` — J2735 SPaT message decoded and published by cpp_message, to be converted to internal format.
* `/incoming_j2735_map` : `j2735_v2x_msgs/msg/MapData` — J2735 MapData message decoded and published by cpp_message, to be converted to internal format.
* `/incoming_j3224_sdsm` : `j3224_v2x_msgs/msg/SensorDataSharingMessage` — J3224 SDSM decoded and published by cpp_message, to be converted to internal format.
* `/outgoing_sdsm` : `carma_v2x_msgs/msg/SensorDataSharingMessage` — Internal SDSM to be converted to J3224 format for outbound transmission.
* `/incoming_j2735_geofence_control` : `j2735_v2x_msgs/msg/TrafficControlMessage` — J2735 traffic control message decoded and published by cpp_message, to be converted to internal format.
* `/incoming_j2735_geofence_request` : `j2735_v2x_msgs/msg/TrafficControlRequest` — J2735 traffic control request decoded and published by cpp_message, to be converted to internal format.
* `/outgoing_geofence_control` : `carma_v2x_msgs/msg/TrafficControlMessage` — Internal traffic control message to be converted to J2735 format for outbound transmission.
* `/outgoing_geofence_request` : `carma_v2x_msgs/msg/TrafficControlRequest` — Internal traffic control request to be converted to J2735 format for outbound transmission.

---

## Parameters

j2735_convertor does not have parameters.

---

## Deployment Instructions

### Build from Source

#### Note: There is no docker image for j2735_convertor alone

1. Create workspace and source directory, or enter it if it already exsists.

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
colcon build --packages-up-to j2735_convertor
```

5. Launch the node.

```sh
cd .. # back into your workspace root
source install/setup.bash
ros2 launch j2735_convertor j2735_convertor_launch.py
```
