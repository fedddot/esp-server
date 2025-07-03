FROM debian:bookworm-slim

ARG DEPS_SRC_DIR=/usr/deps/src

RUN apt-get update
RUN apt-get install -y build-essential libssl-dev zlib1g-dev libbz2-dev \
    libreadline-dev libsqlite3-dev wget curl llvm \
    libncurses5-dev libncursesw5-dev xz-utils tk-dev \
    libffi-dev liblzma-dev python3-openssl git
RUN apt-get install -y python3 python3-pip python3-venv

RUN apt-get install -y flex bison gperf ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

ENV ESP_IDF_PATH=/usr/app/esp-idf
RUN git clone -b v5.3.2 --recursive https://github.com/espressif/esp-idf.git ${ESP_IDF_PATH}
RUN cd ${ESP_IDF_PATH} && ./install.sh esp32 esp32s2

RUN echo "alias get_idf='. ${DEPS_SRC_DIR}/esp-idf/export.sh'" >> /etc/skel/.bashrc