#  Copyright (C) 2018-2025 LEIDOS.
#
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

ARG DOCKER_ORG="usdotfhwastoldev"
ARG DOCKER_TAG="develop-humble"
FROM ${DOCKER_ORG}/carma-base:${DOCKER_TAG} as base_image
COPY --chown=carma . /home/carma/src/

FROM base_image as setup
ARG GIT_BRANCH="develop-humble"

RUN ~/src/docker/checkout.bash -b ${GIT_BRANCH}

RUN ~/src/docker/install.sh

RUN rm -rf /home/carma/src/

ARG BUILD_DATE="NULL"
ARG VERSION="NULL"
ARG VCS_REF="NULL"

LABEL org.label-schema.schema-version="1.0"
LABEL org.label-schema.name="v2x-ros-conversion"
LABEL org.label-schema.description="V2X-ros-conversion docker image"
LABEL org.label-schema.vendor="Leidos"
LABEL org.label-schema.version=${VERSION}
LABEL org.label-schema.url="https://highways.dot.gov/research/research-programs/operations/CARMA"
LABEL org.label-schema.vcs-url="https://github.com/usdot-fhwa-stol/carma-messenger"
LABEL org.label-schema.vcs-ref=${VCS_REF}
LABEL org.label-schema.build-date=${BUILD_DATE}

RUN sudo chmod -R +x /opt/carma/install

RUN pip install future

CMD  [ "wait-for-it", "localhost:11311", "--", "source", "/opt/carma/install/setup.bash", "&&", "ros2", "v2x-ros-conversion", "v2x-ros-conversion.launch.py"]
