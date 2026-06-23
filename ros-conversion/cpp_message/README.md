# cpp_message

The `cpp_message` is a ROS2 package for converting [carma_driver_msgs/msg/ByteArray](https://github.com/usdot-fhwa-stol/carma-msgs/blob/develop/carma_driver_msgs/msg/ByteArray.msg) containing UPER encoded binary into ROS messages [j2735_v2x_msgs/msg](https://github.com/usdot-fhwa-stol/carma-msgs/tree/develop/j2735_v2x_msgs) or [j3224_v2x_msgs/msg](https://github.com/usdot-fhwa-stol/carma-msgs/tree/develop/j3224_v2x_msgs) which match their corresponding SAE schemas. Some messages can be decoded directly into [carma_v2x_msgs/msg](https://github.com/usdot-fhwa-stol/carma-msgs/tree/develop/carma_v2x_msgs). Supported message types include BSM, PSM, SPaT, MAP, SDSM, Traffic Control, and CARMA Mobility messages.

---

## ROS Topics

### Published

Topics published after decoding inbound UPER-encoded binary. J2735/J3224 messages are forwarded to `j2735_convertor`; CARMA messages are ready for direct use.

| Topic | Message Type | Description |
|---|---|---|
| `/outbound_binary_msg` | `carma_driver_msgs/msg/ByteArray` | UPER-encoded binary V2X data ready for transmission via the v2x-ros-driver. |
| `/incoming_j2735_bsm` | `j2735_v2x_msgs/msg/BSM` | Basic Safety Message: vehicle kinematics, signals, emergency info, etc. |
| `/incoming_j2735_spat` | `j2735_v2x_msgs/msg/SPAT` | Signal Phase and Timing: current and predicted signal phase, state, and timing for an intersection's movements. |
| `/incoming_j2735_map` | `j2735_v2x_msgs/msg/MapData` | MapData: conveys one or more intersection lane geometry maps within a single message. |
| `/incoming_j2735_psm` | `j2735_v2x_msgs/msg/PSM` | Personal Safety Message: information on and related to the state of vulnerable road users. |
| `/incoming_j3224_sdsm` | `j3224_v2x_msgs/msg/SensorDataSharingMessage` | Sensor Data Sharing Message: detected objects and senor data. |
| `/incoming_j2735_geofence_request` | `j2735_v2x_msgs/msg/TrafficControlRequest` | Request to setup a virtual, gps boundary. |
| `/incoming_j2735_geofence_control` | `j2735_v2x_msgs/msg/TrafficControlMessage` | Geofence geometry, response, and event data. |
| `/incoming_mobility_operation` | `carma_v2x_msgs/msg/MobilityOperation` | Strategy info for coordination with other Connected Autonomous Vehicles (CAVs). |
| `/incoming_mobility_request` | `carma_v2x_msgs/msg/MobilityRequest` | Request to other CAVs to follow a proposed plan with location, trajectory, etc. |
| `/incoming_mobility_response` | `carma_v2x_msgs/msg/MobilityResponse` | Response to mobility request containing accepted status, reasoning, etc. |
| `/incoming_mobility_path` | `carma_v2x_msgs/msg/MobilityPath` | heartbeat message to broadcast a CAVs current planned 6-second trajectory in ECEF frame at a low frequency. |
| `/incoming_emergency_vehicle_ack` | `carma_v2x_msgs/msg/EmergencyVehicleAck` | used by an emergency vehicle (ERV) to acknowledge receipt of an EmergencyVehicleResponse message from a carma-equipped vehicle. |
| `/incoming_emergency_vehicle_response` | `carma_v2x_msgs/msg/EmergencyVehicleResponse` | For other CAVs to tell the ERV if they can make way. |

### Subscribed

Topics consumed for UPER encoding and outbound transmission. J2735/J3224 messages originate from `j2735_convertor`; CARMA messages are sent directly.

| Topic | Message Type | Description |
|---|---|---|
| `/inbound_binary_msg` | `carma_driver_msgs/msg/ByteArray` | Raw UPER-encoded V2X binary received from the v2x-ros-driver for decoding. |
| `/outgoing_j2735_bsm` | `j2735_v2x_msgs/msg/BSM` | Basic Safety Message to encode and transmit. |
| `/outgoing_j2735_psm` | `j2735_v2x_msgs/msg/PSM` | Personal Safety Message to encode and transmit. |
| `/outgoing_j3224_sdsm` | `j3224_v2x_msgs/msg/SensorDataSharingMessage` | Sensor Data Sharing Message to encode and transmit. |
| `/outgoing_j2735_geofence_request` | `j2735_v2x_msgs/msg/TrafficControlRequest` | Traffic control geofence request to encode and transmit. |
| `/outgoing_j2735_geofence_control` | `j2735_v2x_msgs/msg/TrafficControlMessage` | Traffic control geofence message to encode and transmit. |
| `/outgoing_mobility_operation` | `carma_v2x_msgs/msg/MobilityOperation` | CARMA cooperative operation coordination to encode and transmit. |
| `/outgoing_mobility_request` | `carma_v2x_msgs/msg/MobilityRequest` | CARMA lane change or merge request to encode and transmit. |
| `/outgoing_mobility_response` | `carma_v2x_msgs/msg/MobilityResponse` | CARMA mobility response to encode and transmit. |
| `/outgoing_mobility_path` | `carma_v2x_msgs/msg/MobilityPath` | CARMA trajectory path to encode and transmit. |
| `/outgoing_emergency_vehicle_ack` | `carma_v2x_msgs/msg/EmergencyVehicleAck` | Emergency vehicle acknowledgement to encode and transmit. |
| `/outgoing_emergency_vehicle_response` | `carma_v2x_msgs/msg/EmergencyVehicleResponse` | Emergency vehicle response to encode and transmit. |

---

## Parameters

cpp_message does not have parameters.

---

## Deployment Instructions

### Build from Source

#### Note: There is no docker image for j2735_convertor alone

1. Create workspace and source directory, or enter it if it already exists.

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
