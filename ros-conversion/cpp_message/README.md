# cpp_message

The `cpp_message` is a ROS2 package handling UPER encoding and decoding of V2X messages, converting between structured ROS2 message types and [carma_driver_msgs/msg/ByteArray](https://github.com/usdot-fhwa-stol/carma-msgs/blob/develop/carma_driver_msgs/msg/ByteArray.msg) messages for transmission and reception via a V2X radio driver. Supported message types include BSM, PSM, SPaT, MAP, SDSM, Traffic Control, and CARMA Mobility messages.

## ROS Topics

### Published

* `/outbound_binary_msg` : `carma_driver_msgs/msg/ByteArray` — UPER-encoded binary message ready for transmission via the V2X driver.
* `/incoming_j2735_geofence_request` : `j2735_v2x_msgs/msg/TrafficControlRequest` — Decoded traffic control geofence request, published for consumption by j2735_convertor.
* `/incoming_j2735_geofence_control` : `j2735_v2x_msgs/msg/TrafficControlMessage` — Decoded traffic control geofence message, published for consumption by j2735_convertor.
* `/incoming_mobility_operation` : `carma_v2x_msgs/msg/MobilityOperation` — Decoded mobility operation coordination message from inbound binary data.
* `/incoming_emergency_vehicle_ack` : `carma_v2x_msgs/msg/EmergencyVehicleAck` — Decoded emergency vehicle acknowledgement message from inbound binary data.
* `/incoming_emergency_vehicle_response` : `carma_v2x_msgs/msg/EmergencyVehicleResponse` — Decoded emergency vehicle response message from inbound binary data.
* `/incoming_mobility_response` : `carma_v2x_msgs/msg/MobilityResponse` — Decoded mobility response message from inbound binary data.
* `/incoming_mobility_path` : `carma_v2x_msgs/msg/MobilityPath` — Decoded mobility path trajectory message from inbound binary data.
* `/incoming_mobility_request` : `carma_v2x_msgs/msg/MobilityRequest` — Decoded mobility request message from inbound binary data.
* `/incoming_j2735_bsm` : `j2735_v2x_msgs/msg/BSM` — Decoded J2735 Basic Safety Message, published for consumption by j2735_convertor.
* `/incoming_j2735_spat` : `j2735_v2x_msgs/msg/SPAT` — Decoded J2735 Signal Phase and Timing message, published for consumption by j2735_convertor.
* `/incoming_j2735_map` : `j2735_v2x_msgs/msg/MapData` — Decoded J2735 MapData message, published for consumption by j2735_convertor.
* `/incoming_j2735_psm` : `j2735_v2x_msgs/msg/PSM` — Decoded J2735 Personal Safety Message, published for consumption by j2735_convertor.
* `/incoming_j3224_sdsm` : `j3224_v2x_msgs/msg/SensorDataSharingMessage` — Decoded J3224 Sensor Data Sharing Message, published for consumption by j2735_convertor.

### Subscribed

* `/inbound_binary_msg` : `carma_driver_msgs/msg/ByteArray` — UPER-encoded binary V2X message received from the V2X driver to be decoded.
* `/outgoing_j2735_geofence_request` : `j2735_v2x_msgs/msg/TrafficControlRequest` — Outbound traffic control geofence request to be UPER-encoded, received from j2735_convertor.
* `/outgoing_j2735_geofence_control` : `j2735_v2x_msgs/msg/TrafficControlMessage` — Outbound traffic control geofence message to be UPER-encoded, received from j2735_convertor.
* `/outgoing_mobility_operation` : `carma_v2x_msgs/msg/MobilityOperation` — Outbound mobility operation coordination message to be UPER-encoded.
* `/outgoing_emergency_vehicle_ack` : `carma_v2x_msgs/msg/EmergencyVehicleAck` — Outbound emergency vehicle acknowledgement message to be UPER-encoded.
* `/outgoing_emergency_vehicle_response` : `carma_v2x_msgs/msg/EmergencyVehicleResponse` — Outbound emergency vehicle response message to be UPER-encoded.
* `/outgoing_mobility_response` : `carma_v2x_msgs/msg/MobilityResponse` — Outbound mobility response message to be UPER-encoded.
* `/outgoing_mobility_path` : `carma_v2x_msgs/msg/MobilityPath` — Outbound mobility path trajectory message to be UPER-encoded.
* `/outgoing_mobility_request` : `carma_v2x_msgs/msg/MobilityRequest` — Outbound mobility request message to be UPER-encoded.
* `/outgoing_j2735_bsm` : `j2735_v2x_msgs/msg/BSM` — Outbound J2735 Basic Safety Message to be UPER-encoded, received from j2735_convertor.
* `/outgoing_j2735_psm` : `j2735_v2x_msgs/msg/PSM` — Outbound J2735 Personal Safety Message to be UPER-encoded, received from j2735_convertor.
* `/outgoing_j3224_sdsm` : `j3224_v2x_msgs/msg/SensorDataSharingMessage` — Outbound J3224 Sensor Data Sharing Message to be UPER-encoded, received from j2735_convertor.

## Parameters

cpp_message does not have parameters.

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
colcon build --packages-up-to cpp_message
```

5. Launch the node.

```sh
cd .. # back into your workspace root
source install/setup.bash
ros2 launch cpp_message cpp_message_launch.py
```
