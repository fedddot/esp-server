FROM espressif/idf:latest

RUN apt-get update
RUN apt-get install -y lsb-release software-properties-common gnupg
RUN apt-get install -y bash-completion
RUN apt-get install -y protobuf-compiler
RUN apt-get install -y locales

RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8
ENV LANG="en_US.UTF-8" 

RUN ${IDF_PATH}/tools/idf_tools.py install esp-clang
ENV IDF_PYTHON_ENV_PATH=${IDF_TOOLS_PATH}/python_env/idf6.0_py3.12_env
RUN bash --init-file ${IDF_PYTHON_ENV_PATH}/bin/activate -c "${IDF_PYTHON_ENV_PATH}/bin/pip install protobuf grpcio-tools"

ENV TARGET=esp32s2
ENV CLANGD_FLAGS="--query-driver=/opt/esp/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/xtensa-${TARGET}-elf-gcc,/opt/esp/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/xtensa-${TARGET}-elf-g++"

WORKDIR /usr/app/external/googletest
RUN wget https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
ENV GOOGLE_TEST_ARCHIVE_PATH=/usr/app/external/googletest/v1.15.2.tar.gz

WORKDIR /usr/app/external/jsoncpp
RUN wget https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.6.tar.gz
ENV JSONCPP_ARCHIVE_PATH=/usr/app/external/jsoncpp/1.9.6.tar.gz

WORKDIR /usr/app/external
RUN git clone --branch=0.4.9 https://github.com/nanopb/nanopb.git nanopb
ENV NANOPB_SRC_PATH=/usr/app/external/nanopb

WORKDIR /usr/app/external
RUN git clone --branch=main https://github.com/fedddot/mcu_server.git mcu_server
ENV MCU_SERVER_PATH=/usr/app/external/mcu_server

ENV SHELL=/bin/bash
RUN echo 'bind "\"\e[A\": history-search-backward"' >> ${HOME}/.bashrc
RUN echo 'bind "\"\e[B\": history-search-forward"' >> ${HOME}/.bashrc
RUN echo 'get_idf 1>/dev/null' >> ${HOME}/.bashrc
RUN echo 'source /etc/skel/.bashrc' >> ${HOME}/.bashrc

WORKDIR /project

ENTRYPOINT ["/bin/bash", "-c"]