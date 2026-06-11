# j2735_convertor

This package converts the subscribed j2735 msgs to carma msgs so that they can be parsed by the rest of the carma system.

---

## ROS Topics

### Published

* `/incoming_bsm` : carma_v2x_msgs/msg/BSM - A converted internal CARMA Basic Safety Message (BSM) derived from an incoming J2735 message.
* `/outgoing_j2735_bsm` : j2735_v2x_msgs/msg/BSM - A standard J2735 Basic Safety Message (BSM) ready for outbound transmission.
* `/incoming_psm` : carma_v2x_msgs/msg/PSM - A converted internal CARMA Personal Safety Message (PSM) derived from an incoming J2735 message.
* `/outgoing_j2735_psm` : j2735_v2x_msgs/msg/PSM - A standard J2735 Personal Safety Message (PSM) ready for outbound transmission.
* `/incoming_spat` : carma_v2x_msgs/msg/SPAT - A converted internal CARMA Signal Phase and Timing (SPaT) message derived from an incoming J2735 message.
* `/incoming_map` : carma_v2x_msgs/msg/MapData - A converted internal CARMA MapData message derived from an incoming J2735 message.
* `/incoming_sdsm` : carma_v2x_msgs/msg/SensorDataSharingMessage - A converted internal CARMA Sensor Data Sharing Message (SDSM) derived from an incoming J3224 message.
* `/outgoing_j3224_sdsm` : j3224_v2x_msgs/msg/SensorDataSharingMessage - A standard J3224 Sensor Data Sharing Message (SDSM) ready for outbound transmission.
* `/incoming_geofence_control` : carma_v2x_msgs/msg/TrafficControlMessage - A converted internal CARMA traffic control geofence message (published as transient local).
* `/incoming_geofence_request` : carma_v2x_msgs/msg/TrafficControlRequest - A converted internal CARMA traffic control geofence request message.
* `/outgoing_j2735_geofence_control` : j2735_v2x_msgs/msg/TrafficControlMessage - A standard J2735 traffic control geofence message ready for outbound transmission.
* `/outgoing_j2735_geofence_request` : j2735_v2x_msgs/msg/TrafficControlRequest - A standard J2735 traffic control geofence request message ready for outbound transmission.

### Subscribed

* `/incoming_j2735_bsm` : j2735_v2x_msgs/msg/BSM - A standard J2735 Basic Safety Message (BSM) received from the V2X driver.
* `/outgoing_bsm` : carma_v2x_msgs/msg/BSM - An internal CARMA Basic Safety Message (BSM) intended for outbound translation and broadcast.
* `/incoming_j2735_psm` : j2735_v2x_msgs/msg/PSM - A standard J2735 Personal Safety Message (PSM) received from the V2X driver.
* `/outgoing_psm` : carma_v2x_msgs/msg/PSM - An internal CARMA Personal Safety Message (PSM) intended for outbound translation and broadcast.
* `/incoming_j2735_spat` : j2735_v2x_msgs/msg/SPAT - A standard J2735 Signal Phase and Timing (SPaT) message received from the V2X driver.
* `/incoming_j2735_map` : j2735_v2x_msgs/msg/MapData - A standard J2735 MapData message received from the V2X driver.
* `/incoming_j3224_sdsm` : j3224_v2x_msgs/msg/SensorDataSharingMessage - A standard J3224 Sensor Data Sharing Message (SDSM) received from the V2X driver.
* `/outgoing_sdsm` : carma_v2x_msgs/msg/SensorDataSharingMessage - An internal CARMA Sensor Data Sharing Message (SDSM) intended for outbound translation and broadcast.
* `/incoming_j2735_geofence_control` : j2735_v2x_msgs/msg/TrafficControlMessage - A standard J2735 traffic control geofence message received from the V2X driver.
* `/incoming_j2735_geofence_request` : j2735_v2x_msgs/msg/TrafficControlRequest - A standard J2735 traffic control geofence request message received from the V2X driver.
* `/outgoing_geofence_control` : carma_v2x_msgs/msg/TrafficControlMessage - An internal CARMA traffic control geofence message intended for outbound translation.
* `/outgoing_geofence_request` : carma_v2x_msgs/msg/TrafficControlRequest - An internal CARMA traffic control geofence request message intended for outbound translation.

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
