# v2x-ros-conversion

| CI Build Status | Docker Workflow | Sonar Code Quality | Sonar Coverage | Docker Pulls | Docker Cloud Build Status |
| --- | --- | --- | --- | --- | --- |
| [![CI](https://github.com/usdot-fhwa-stol/v2x-ros-conversion/actions/workflows/ci.yml/badge.svg)](https://github.com/usdot-fhwa-stol/v2x-ros-conversion/actions/workflows/ci.yml) | [![Docker](https://github.com/usdot-fhwa-stol/v2x-ros-conversion/actions/workflows/docker.yml/badge.svg)](https://github.com/usdot-fhwa-stol/v2x-ros-conversion/actions/workflows/docker.yml) | [![SonarCloud Quality](https://sonarcloud.io/api/project_badges/measure?project=usdot-fhwa-stol_v2x-ros-conversion-humble&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=usdot-fhwa-stol_v2x-ros-conversion-humble) | ![Sonar Coverage](https://img.shields.io/sonar/coverage/usdot-fhwa-stol_v2x-ros-conversion-humble?server=https%3A%2F%2Fsonarcloud.io) | [![Docker Pulls](https://img.shields.io/docker/pulls/usdotfhwastoldev/v2x-ros-conversion)](https://hub.docker.com/repository/docker/usdotfhwastoldev/v2x-ros-conversion/general) | ![Docker Cloud Build Status](https://img.shields.io/docker/cloud/build/usdotfhwastoldev/v2x-ros-conversion) |

The v2x-ros-conversion package contains two ROS2 nodes that converts a ROS2 message to a binary message that can be forwarded to a OBU/RSU to broadcast the message. This package uses ROS22 Humble.

---

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

## Contribution
Welcome to the CARMA contributing guide. Please read this guide to learn about our development process, how to propose pull requests and improvements, and how to build and test your changes to this project. [CARMA Contributing Guide](https://github.com/usdot-fhwa-stol/CARMAPlatform/blob/develop/Contributing.md) 

## Code of Conduct 
Please read our [CARMA Code of Conduct](https://github.com/usdot-fhwa-stol/CARMAPlatform/blob/develop/Code_of_Conduct.md) which outlines our expectations for participants within the CARMA community, as well as steps to reporting unacceptable behavior. We are committed to providing a welcoming and inspiring community for all and expect our code of conduct to be honored. Anyone who violates this code of conduct may be banned from the community.

## Attribution
The development team would like to acknowledge the people who have made direct contributions to the design and code in this repository. [CARMA Attribution](https://github.com/usdot-fhwa-stol/carma-platform/blob/develop/ATTRIBUTION.md) 

## License
By contributing to the Federal Highway Administration (FHWA) Connected Automated Research Mobility Applications (CARMA), you agree that your contributions will be licensed under its Apache License 2.0 license. [CARMA License](https://github.com/usdot-fhwa-stol/CARMAPlatform/blob/develop/docs/License.md) 

## Contact
Please click on the CARMA logo below to visit the Federal Highway Adminstration(FHWA) CARMA website. For more information, contact CAVSupportServices@dot.gov.