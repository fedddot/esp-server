FROM gcc:9

# basics
RUN apt-get update
RUN apt-get install -y cmake git g++ gdb-multiarch
RUN apt-get install -y bash-completion
RUN apt-get install -y clang-tools

# esp-idf prereqs
RUN apt-get install -y wget flex bison gperf python3 python3-pip python3-venv ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

# get esp-idf
WORKDIR /usr/app/deps
RUN git clone -b v5.3.1 https://github.com/espressif/esp-idf.git esp

WORKDIR /usr/app/deps/esp
RUN sh install.sh esp32
RUN python3 -u tools/idf_tools.py install
RUN sh export.sh

# get mcu_server
WORKDIR /usr/app/deps
RUN git clone -b main https://github.com/fedddot/mcu_server.git mcu_server
ENV MCU_SERVER_PATH=/usr/app/deps/mcu_server

# server sources should be mapped to this path during container run
WORKDIR /usr/src/app

RUN ln /usr/lib/llvm-11/bin/clang /usr/lib/llvm-11/bin/clangd
ENV PATH=/bin:/usr/bin:/usr/lib/llvm-11/bin
ENV SHELL=/bin/bash
RUN echo -e "\"\e[A\": history-search-backward" >> /etc/skel/.bashrc
RUN echo -e "\"\e[B\": history-search-forward" >> /etc/skel/.bashrc

CMD ["/bin/bash"]
