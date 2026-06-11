# cpp_message

cpp message node used to publish and receive and decode and encode j2735 messages.

---

## ROS Topics

### Published

* `/outbound_binary_msg` : carma_driver_msgs/msg/ByteArray - An UPER-encoded binary message to be transmitted by the V2X driver.
* `/incoming_j2735_geofence_request` : j2735_v2x_msgs/msg/TrafficControlRequest - A received traffic control geofence request message.
* `/incoming_j2735_geofence_control` : j2735_v2x_msgs/msg/TrafficControlMessage - A received traffic control geofence message.
* `/incoming_mobility_operation` : carma_v2x_msgs/msg/MobilityOperation - A received CARMA mobility operation coordination message.
* `/incoming_emergency_vehicle_ack` : carma_v2x_msgs/msg/EmergencyVehicleAck - A received emergency vehicle acknowledgement message.
* `/incoming_emergency_vehicle_response` : carma_v2x_msgs/msg/EmergencyVehicleResponse - A received emergency vehicle response message.
* `/incoming_mobility_response` : carma_v2x_msgs/msg/MobilityResponse - A received CARMA mobility response message.
* `/incoming_mobility_path` : carma_v2x_msgs/msg/MobilityPath - A received CARMA mobility path trajectory message.
* `/incoming_mobility_request` : carma_v2x_msgs/msg/MobilityRequest - A received CARMA mobility request message.
* `/incoming_j2735_bsm` : j2735_v2x_msgs/msg/BSM - A received J2735 Basic Safety Message (BSM).
* `/incoming_j2735_spat` : j2735_v2x_msgs/msg/SPAT - A received J2735 Signal Phase and Timing (SPaT) message.
* `/incoming_j2735_map` : j2735_v2x_msgs/msg/MapData - A received J2735 MapData message.
* `/incoming_j2735_psm` : j2735_v2x_msgs/msg/PSM - A received J2735 Personal Safety Message (PSM).
* `/incoming_j3224_sdsm` : j3224_v2x_msgs/msg/SensorDataSharingMessage - A received J3224 Sensor Data Sharing Message (SDSM).

### Subscribed

* `/inbound_binary_msg` : carma_driver_msgs/msg/ByteArray - An UPER-encoded binary V2X message received from the radio driver to be decoded.
* `/outgoing_j2735_geofence_request` : j2735_v2x_msgs/msg/TrafficControlRequest - An outbound traffic control geofence request message waiting to be encoded.
* `/outgoing_j2735_geofence_control` : j2735_v2x_msgs/msg/TrafficControlMessage - An outbound traffic control geofence message waiting to be encoded.
* `/outgoing_mobility_operation` : carma_v2x_msgs/msg/MobilityOperation - An outbound CARMA mobility operation coordination message waiting to be encoded.
* `/outgoing_emergency_vehicle_ack` : carma_v2x_msgs/msg/EmergencyVehicleAck - An outbound emergency vehicle acknowledgement message waiting to be encoded.
* `/outgoing_emergency_vehicle_response` : carma_v2x_msgs/msg/EmergencyVehicleResponse - An outbound emergency vehicle response message waiting to be encoded.
* `/outgoing_mobility_response` : carma_v2x_msgs/msg/MobilityResponse - An outbound CARMA mobility response message waiting to be encoded.
* `/outgoing_mobility_path` : carma_v2x_msgs/msg/MobilityPath - An outbound CARMA mobility path trajectory message waiting to be encoded.
* `/outgoing_mobility_request` : carma_v2x_msgs/msg/MobilityRequest - An outbound CARMA mobility request message waiting to be encoded.
* `/outgoing_j2735_bsm` : j2735_v2x_msgs/msg/BSM - An outbound J2735 Basic Safety Message (BSM) waiting to be encoded.
* `/outgoing_j2735_psm` : j2735_v2x_msgs/msg/PSM - An outbound J2735 Personal Safety Message (PSM) waiting to be encoded.
* `/outgoing_j3224_sdsm` : j3224_v2x_msgs/msg/SensorDataSharingMessage - An outbound J3224 Sensor Data Sharing Message (SDSM) waiting to be encoded.

---

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
