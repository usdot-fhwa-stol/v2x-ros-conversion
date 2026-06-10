## Architecture

The core components in CARMA Messenger contains two ROS2 nodes: Message node and J2735 Convertor Node. The Message node takes in a ROS2 message and uses asn1c lib generated code to convert it to a binary blob and forwards it to the DSRC driver, such that any OBU/RSU can broadcast the message directly. The J2735 Converter converts the data from a ROS2 message based on the J2735 standard units to another message using more usable SI units and vice-versa.

### Example Setup

```sh
# Clone repo
git clone https://github.com/usdot-fhwa-stol/v2x-ros-conversion.git

# Build development config
cd v2x-ros-conversion/docker
./build-image.sh
```

### Recommended Setup

To incorporate the v2x-ros-conversion package into a docker project, call the following command to clone the v2x-ros-conversion package into the project:

```sh
git clone --depth=1 https://github.com/usdot-fhwa-stol/v2x-ros-conversion.git ${dir}/src/v2x-ros-conversion --branch "develop"
```

Once installed, run the build image script for the v2x-ros-conversion package. This will also install all the necessary prerequisites needed for the package.

---