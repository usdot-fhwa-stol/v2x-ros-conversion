#!/bin/sh

# exit on errors
set -e

# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
. /etc/lsb-release

# add the STOL APT repository
echo "deb [trusted=yes] http://s3.amazonaws.com/stol-apt-repository ${DISTRIB_CODENAME} main" | sudo tee /etc/apt/sources.list.d/stol-apt-repository.list

sudo apt-get update

# install all things needed for deployment, always done
sudo apt-get install -y carma-j2735-1
