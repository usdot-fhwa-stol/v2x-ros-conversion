# v2x-ros-conversion

The V2X-ROS-Conversion package contains two ROS nodes origionally developed and packaged inside the CARMA Platform communication stack: Message node and J2735 Convertor Node. These two packages were ported to a seperate repository to allow for other projects to utilize these ROS nodes.

## Architecture

The core components in CARMA Messenger contains two ROS nodes origionally developed for the CARMA Platform communication stack: Message node and J2735 Convertor Node. The Message node takes in a ROS message and uses asn1c lib generated code to convert it to a binary blob and forwards it to the DSRC driver, such that any OBU/RSU can broadcast the message directly. The J2735 Converter converts the data from a ROS message based on the J2735 standard units to another message using more usable SI units and vice-versa.

### Example Setup

```
# Clone repo
git clone https://github.com/usdot-fhwa-stol/v2x-ros-conversion.git

# Build development config
cd v2x-ros-conversion/docker
./build-image.sh

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
