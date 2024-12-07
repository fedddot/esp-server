FROM debian:buster

ARG DEPS_SRC_DIR=/usr/deps/src

RUN apt-get update
RUN apt-get install -y build-essential libssl-dev zlib1g-dev libbz2-dev \
    libreadline-dev libsqlite3-dev wget curl llvm \
    libncurses5-dev libncursesw5-dev xz-utils tk-dev \
    libffi-dev liblzma-dev python3-openssl git

WORKDIR ${DEPS_SRC_DIR}
RUN wget https://www.python.org/ftp/python/3.8.16/Python-3.8.16.tgz && \
    tar -xf Python-3.8.16.tgz && \
    rm Python-3.8.16.tgz && \
    cd Python-3.8.16 && \
    ./configure --enable-optimizations && \
    make -j4 && \
    make install
RUN apt-get install -y python3-pip python3-venv 

RUN apt-get install -y flex bison gperf ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

ARG DEPS_TOOLS_DIR=/usr/deps/tools
ENV IDF_TOOLS_PATH=${DEPS_TOOLS_DIR}/esp_tools
WORKDIR ${DEPS_SRC_DIR}
RUN git clone -b v5.3.2 --recursive https://github.com/espressif/esp-idf.git esp-idf
RUN cd ${DEPS_SRC_DIR}/esp-idf && ./install.sh esp32 esp32s2

RUN echo "alias get_idf='. ${DEPS_SRC_DIR}/esp-idf/export.sh'" >> /etc/skel/.bashrc