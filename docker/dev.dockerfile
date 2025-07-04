FROM espressif/idf:latest

RUN apt-get update
RUN apt-get install -y lsb-release software-properties-common gnupg
RUN apt-get install -y bash-completion
RUN apt-get install -y protobuf-compiler
RUN apt-get install -y locales

ENV MCU_SERVER_PATH=/usr/app/mcu_server
RUN git clone -b main https://github.com/fedddot/mcu_server.git ${MCU_SERVER_PATH}

ENV SHELL=/bin/bash
RUN echo 'bind "\"\e[A\": history-search-backward"' >> /etc/skel/.bashrc
RUN echo 'bind "\"\e[B\": history-search-forward"' >> /etc/skel/.bashrc
RUN echo 'get_idf 1>/dev/null' >> /etc/skel/.bashrc
RUN echo 'source /etc/skel/.bashrc' >> /root/.bashrc

RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8
ENV LANG="en_US.UTF-8" 

RUN ${IDF_PATH}/tools/idf_tools.py install esp-clang

ENV TARGET=esp32s2
ENV CLANGD_FLAGS="--query-driver=/opt/esp/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/xtensa-${TARGET}-elf-gcc,/opt/esp/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/xtensa-${TARGET}-elf-g++"

CMD ["/bin/bash"]