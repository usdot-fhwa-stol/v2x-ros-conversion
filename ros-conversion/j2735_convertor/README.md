# j2735_convertor

The `j2735_convertor` is a ROS2 package providing bidirectional conversion between standard J2735/J3224 V2X message types and their internal ROS2 representations, supporting BSM, PSM, SPaT, MAP, SDSM, and Traffic Control messages.

---

## ROS Topics

### Published

* `/incoming_bsm` : `carma_v2x_msgs/msg/BSM` — Converted internal CARMA Basic Safety Message (BSM) derived from an incoming J2735 message.
* `/outgoing_j2735_bsm` : `j2735_v2x_msgs/msg/BSM` — Standard J2735 Basic Safety Message (BSM) ready for outbound transmission.
* `/incoming_psm` : `carma_v2x_msgs/msg/PSM` — Converted internal CARMA Personal Safety Message (PSM) derived from an incoming J2735 message.
* `/outgoing_j2735_psm` : `j2735_v2x_msgs/msg/PSM` — Standard J2735 Personal Safety Message (PSM) ready for outbound transmission.
* `/incoming_spat` : `carma_v2x_msgs/msg/SPAT` — Converted internal CARMA Signal Phase and Timing (SPaT) message derived from an incoming J2735 message.
* `/incoming_map` : `carma_v2x_msgs/msg/MapData` — Converted internal CARMA MapData message derived from an incoming J2735 message.
* `/incoming_sdsm` : `carma_v2x_msgs/msg/SensorDataSharingMessage` — Converted internal CARMA Sensor Data Sharing Message (SDSM) derived from an incoming J3224 message.
* `/outgoing_j3224_sdsm` : `j3224_v2x_msgs/msg/SensorDataSharingMessage` — Standard J3224 Sensor Data Sharing Message (SDSM) ready for outbound transmission.
* `/incoming_geofence_control` : `carma_v2x_msgs/msg/TrafficControlMessage` — Converted internal CARMA traffic control message derived from an incoming J2735 message.
* `/incoming_geofence_request` : `carma_v2x_msgs/msg/TrafficControlRequest` — Converted internal CARMA traffic control request derived from an incoming J2735 message.
* `/outgoing_j2735_geofence_control` : `j2735_v2x_msgs/msg/TrafficControlMessage` — Standard J2735 traffic control message ready for outbound transmission.
* `/outgoing_j2735_geofence_request` : `j2735_v2x_msgs/msg/TrafficControlRequest` — Standard J2735 traffic control request ready for outbound transmission.

### Subscribed

* `/incoming_j2735_bsm` : `j2735_v2x_msgs/msg/BSM` — Standard J2735 Basic Safety Message (BSM) received from the V2X driver.
* `/outgoing_bsm` : `carma_v2x_msgs/msg/BSM` — Internal CARMA Basic Safety Message (BSM) to be translated and broadcast outbound.
* `/incoming_j2735_psm` : `j2735_v2x_msgs/msg/PSM` — Standard J2735 Personal Safety Message (PSM) received from the V2X driver.
* `/outgoing_psm` : `carma_v2x_msgs/msg/PSM` — Internal CARMA Personal Safety Message (PSM) to be translated and broadcast outbound.
* `/incoming_j2735_spat` : `j2735_v2x_msgs/msg/SPAT` — Standard J2735 Signal Phase and Timing (SPaT) message received from the V2X driver.
* `/incoming_j2735_map` : `j2735_v2x_msgs/msg/MapData` — Standard J2735 MapData message received from the V2X driver.
* `/incoming_j3224_sdsm` : `j3224_v2x_msgs/msg/SensorDataSharingMessage` — Standard J3224 Sensor Data Sharing Message (SDSM) received from the V2X driver.
* `/outgoing_sdsm` : `carma_v2x_msgs/msg/SensorDataSharingMessage` — Internal CARMA Sensor Data Sharing Message (SDSM) to be translated and broadcast outbound.
* `/incoming_j2735_geofence_control` : `j2735_v2x_msgs/msg/TrafficControlMessage` — Standard J2735 traffic control message received from the V2X driver.
* `/incoming_j2735_geofence_request` : `j2735_v2x_msgs/msg/TrafficControlRequest` — Standard J2735 traffic control request received from the V2X driver.
* `/outgoing_geofence_control` : `carma_v2x_msgs/msg/TrafficControlMessage` — Internal CARMA traffic control message to be translated and broadcast outbound.
* `/outgoing_geofence_request` : `carma_v2x_msgs/msg/TrafficControlRequest` — Internal CARMA traffic control request to be translated and broadcast outbound.

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
