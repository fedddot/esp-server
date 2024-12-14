FROM esp-base:latest

RUN apt-get update
RUN apt-get install -y gcc g++ gdb-multiarch
RUN apt-get install -y bash-completion
RUN apt-get install -y clangd-13
RUN apt-get install -y protobuf-compiler

ARG DEPS_SRC_DIR=/usr/deps/src

WORKDIR ${DEPS_SRC_DIR}
RUN wget https://cmake.org/files/v3.28/cmake-3.28.1.tar.gz && \
    tar -xzvf cmake-3.28.1.tar.gz && \
    rm cmake-3.28.1.tar.gz && \
    cd cmake-3.28.1 && \
    ./bootstrap && \
    make -j4 && \
    make install

RUN git clone -b main https://github.com/fedddot/mcu_server.git mcu_server
ENV MCU_SERVER_PATH=${DEPS_SRC_DIR}/mcu_server

ENV SHELL=/bin/bash
ENV PATH=${PATH}:/usr/lib/llvm-13/bin:/usr/bin
RUN echo 'bind "\"\e[A\": history-search-backward"' >> /etc/skel/.bashrc
RUN echo 'bind "\"\e[B\": history-search-forward"' >> /etc/skel/.bashrc
RUN echo 'get_idf 1>/dev/null' >> /etc/skel/.bashrc
RUN echo 'source /etc/skel/.bashrc' >> /root/.bashrc

RUN apt-get install -y locales
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8
ENV LANG="en_US.UTF-8" 

CMD ["/bin/bash"]