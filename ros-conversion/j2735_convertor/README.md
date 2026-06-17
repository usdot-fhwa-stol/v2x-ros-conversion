# j2735_convertor

The `j2735_convertor` is a ROS2 package providing bidirectional conversion between standard [J2735](https://github.com/usdot-fhwa-stol/carma-msgs/tree/develop/j2735_v2x_msgs)/[J3224](https://github.com/usdot-fhwa-stol/carma-msgs/tree/develop/j3224_v2x_msgs) V2X message types and their internal [CARMA](https://github.com/usdot-fhwa-stol/carma-msgs/tree/develop/carma_v2x_msgs) ROS2 representations. Supported message types: BSM, PSM, SPaT, MAP, SDSM, and Traffic Control.

## ROS Topics

Topics with prefixes: `incoming_j2735_*` / `incoming_j3224_*` carry J2735/J3224-format messages from `cpp_message`; `incoming_*` carry converted internal messages to the rest of the system; `outgoing_*` flow in the opposite direction.

### Published

| Topic | Message Type | Description |
|---|---|---|
| `/incoming_bsm` | `carma_v2x_msgs/msg/BSM` | Basic Safety Message: vehicle kinematics, signals, emergency info, etc. |
| `/incoming_psm` | `carma_v2x_msgs/msg/PSM` | Personal Safety Message: state and location of vulnerable road users. |
| `/incoming_spat` | `carma_v2x_msgs/msg/SPAT` | Signal Phase and Timing: description of an intersection's roadway geometry and its allowed navigational paths. |
| `/incoming_map` | `carma_v2x_msgs/msg/MapData` | MapData: intersection lane geometry maps. |
| `/incoming_sdsm` | `carma_v2x_msgs/msg/SensorDataSharingMessage` | Sensor Data Sharing Message: detected objects and sensor data. |
| `/incoming_geofence_control` | `carma_v2x_msgs/msg/TrafficControlMessage` | Geofence geometry, response, and event data. |
| `/incoming_geofence_request` | `carma_v2x_msgs/msg/TrafficControlRequest` | Request to set up a virtual GPS boundary. |
| `/outgoing_j2735_bsm` | `j2735_v2x_msgs/msg/BSM` | Basic Safety Message in J2735 format, forwarded to `cpp_message` for encoding. |
| `/outgoing_j2735_psm` | `j2735_v2x_msgs/msg/PSM` | Personal Safety Message in J2735 format, forwarded to `cpp_message` for encoding. |
| `/outgoing_j3224_sdsm` | `j3224_v2x_msgs/msg/SensorDataSharingMessage` | SDSM in J3224 format, forwarded to `cpp_message` for encoding. |
| `/outgoing_j2735_geofence_control` | `j2735_v2x_msgs/msg/TrafficControlMessage` | Traffic control message in J2735 format, forwarded to `cpp_message` for encoding. |
| `/outgoing_j2735_geofence_request` | `j2735_v2x_msgs/msg/TrafficControlRequest` | Traffic control request in J2735 format, forwarded to `cpp_message` for encoding. |

### Subscribed

| Topic | Message Type | Description |
|---|---|---|
| `/incoming_j2735_bsm` | `j2735_v2x_msgs/msg/BSM` | Basic Safety Message decoded by `cpp_message`, to be converted to internal format. |
| `/incoming_j2735_psm` | `j2735_v2x_msgs/msg/PSM` | Personal Safety Message decoded by `cpp_message`, to be converted to internal format. |
| `/incoming_j2735_spat` | `j2735_v2x_msgs/msg/SPAT` | SPaT message decoded by `cpp_message`, to be converted to internal format. |
| `/incoming_j2735_map` | `j2735_v2x_msgs/msg/MapData` | MapData decoded by `cpp_message`, to be converted to internal format. |
| `/incoming_j3224_sdsm` | `j3224_v2x_msgs/msg/SensorDataSharingMessage` | SDSM decoded by `cpp_message`, to be converted to internal format. |
| `/incoming_j2735_geofence_control` | `j2735_v2x_msgs/msg/TrafficControlMessage` | Traffic control message decoded by `cpp_message`, to be converted to internal format. |
| `/incoming_j2735_geofence_request` | `j2735_v2x_msgs/msg/TrafficControlRequest` | Traffic control request decoded by `cpp_message`, to be converted to internal format. |
| `/outgoing_bsm` | `carma_v2x_msgs/msg/BSM` | Basic Safety Message to be converted to J2735 format for outbound transmission. |
| `/outgoing_psm` | `carma_v2x_msgs/msg/PSM` | Personal Safety Message to be converted to J2735 format for outbound transmission. |
| `/outgoing_sdsm` | `carma_v2x_msgs/msg/SensorDataSharingMessage` | SDSM to be converted to J3224 format for outbound transmission. |
| `/outgoing_geofence_control` | `carma_v2x_msgs/msg/TrafficControlMessage` | Traffic control message to be converted to J2735 format for outbound transmission. |
| `/outgoing_geofence_request` | `carma_v2x_msgs/msg/TrafficControlRequest` | Traffic control request to be converted to J2735 format for outbound transmission. |

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
