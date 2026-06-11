# v2x-ros-conversion

The v2x-ros-driver is a ros2 package currently implemented in ros2-humble. but previous releases support ros2-foxy. The v2x-ros-driver contains two ROS2 nodes: Message node and J2735 Convertor Node. They handle the conversion between decoded (i.e., structured, human readable) V2X ROS messages and [carma_driver_msgs/msg/ByteArray](https://github.com/usdot-fhwa-stol/carma-msgs/blob/develop/carma_driver_msgs/msg/ByteArray.msg) messages containing UPER-encoded V2X data that can be forwarded to a OBU/RSU to broadcast the message.

---

## Deployment Instructions

### Deploy using docker (recommended)

1. Pull the latest docker image for conversion from dockerhub

```sh
docker pull usdotfhwastoldev/v2x-ros-conversion:develop
```

#### Note: Versions up to carma-system-4.5.0 (ros2-foxy supported) can all be found under [usdotfhwastol/carma-cohda-dsrc-driver<release-tag>](https://hub.docker.com/r/usdotfhwastol/carma-cohda-dsrc-driver)

2. Run the Docker image

```sh
docker run -it --network host usdotfhwastoldev/v2x-ros-conversion:develop
```

---

## Build from Source

### Build Standalone

1. Clone the repository into workspace.

```sh
git clone https://github.com/usdot-fhwa-stol/v2x-ros-conversion.git
```

2. Run image build script.

```sh
cd v2x-ros-conversion/docker
./build-image.sh
```

### Build Into an Exsisting project

1. Clone the v2x-ros-conversion package into the project:

```sh
git clone --depth=1 https://github.com/usdot-fhwa-stol/v2x-ros-conversion.git ${dir}/src/v2x-ros-conversion --branch "develop"
```

2. Run the build image script for the v2x-ros-conversion package.

```sh
cd src/v2x-ros-conversion/docker
./build-image.sh
```
