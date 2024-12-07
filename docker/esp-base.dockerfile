FROM debian:buster

# prereqs
RUN apt-get update
RUN apt-get install -y wget flex bison gperf python3 python3-pip python3-venv ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
RUN apt-get install -y git

ARG DEPS_SRC_DIR=/usr/deps/src
ARG DEPS_TOOLS_DIR=/usr/deps/tools
ENV IDF_TOOLS_PATH=${DEPS_TOOLS_DIR}/esp_tools

WORKDIR ${DEPS_SRC_DIR}
RUN git clone -b v5.3.2 --recursive https://github.com/espressif/esp-idf.git
RUN cd ${DEPS_SRC_DIR}/esp-idf && ./install.sh esp32 esp32s2

RUN echo -e "alias get_idf='. ${DEPS_SRC_DIR}/esp-idf/export.sh'" >> /etc/skel/.bashrc