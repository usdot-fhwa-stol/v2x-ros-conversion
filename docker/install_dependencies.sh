#!/bin/bash

#  Copyright (C) 2018-2024 LEIDOS. 
#  Licensed under the Apache License, Version 2.0 (the "License"); you may not
#  use this file except in compliance with the License. You may obtain a copy of
#  the License at
# 
#  http://www.apache.org/licenses/LICENSE-2.0
# 
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
#  License for the specific language governing permissions and limitations under
#  the License.

# CARMA packages checkout script
# Optional argument to set the root checkout directory with no ending '/' default is '~'

set -e

BRANCH=develop  # The script will use this unless the -b flag updates it
while [[ $# -gt 0 ]]; do
      arg="$1"
      case $arg in
            -b|--branch)
                  BRANCH=$2
                  shift
                  shift
            ;;
      esac
done

# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
. /etc/lsb-release
NO_UNDERSCORE_NAME=${BRANCH//_/-}
if [[ "${BRANCH}" =~ feature/.* ]]; then
    APT_CODENAME=feature-${NO_UNDERSCORE_NAME##*/}
# check for release candidate branches
elif [[ "${BRANCH}" =~ release/.* ]]; then
    APT_CODENAME=candidate-${NO_UNDERSCORE_NAME##*/}
# check for release tags
elif [[ "${BRANCH}" =~ carma-system* ]]; then
    APT_CODENAME=main
elif [[ "${BRANCH}" =~ develop ]] ; then
    APT_CODENAME="develop"
else 
    APT_CODENAME="develop"
fi
# add the STOL APT repository
echo "deb [trusted=yes] http://s3.amazonaws.com/stol-apt-repository ${APT_CODENAME} ${DISTRIB_CODENAME} " | sudo tee /etc/apt/sources.list.d/stol-apt-repository.list

sudo apt-get update

# install all things needed for deployment, always done
sudo apt-get install -y stol-j2735-201603-carma-1
