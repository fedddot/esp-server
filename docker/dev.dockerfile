FROM gcc:9

# basics
RUN apt-get update
RUN apt-get install -y cmake git g++ gdb-multiarch
RUN apt-get install -y bash-completion

# esp-idf prereqs
RUN apt-get install -y wget flex bison gperf python3 python3-pip python3-venv ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

# get esp-idf
WORKDIR /usr/app/deps
RUN git clone -b v5.3.1 --recursive https://github.com/espressif/esp-idf.git esp
RUN cd esp && ./install.sh esp32

# get mcu_server
WORKDIR /usr/app/deps
RUN git clone -b main https://github.com/fedddot/mcu_server.git mcu_server
ENV MCU_SERVER_PATH=/usr/app/deps/mcu_server

# server sources should be mapped to this path during container run
WORKDIR /usr/src/app

ENV SHELL=/bin/bash

CMD ["/bin/bash"]
