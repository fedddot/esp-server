FROM esp-base:latest

RUN apt-get update
RUN apt-get install -y gcc g++ cmake gdb-multiarch
RUN apt-get install -y bash-completion
RUN apt-get install -y clangd-13

WORKDIR ${DEPS_SRC_DIR}
RUN git clone -b main https://github.com/fedddot/mcu_server.git mcu_server
ENV MCU_SERVER_PATH=${DEPS_SRC_DIR}/mcu_server

ENV SHELL=/bin/bash
ENV PATH=${PATH}:/usr/lib/llvm-13/bin
RUN echo -e "\"\e[A\": history-search-backward" >> /etc/skel/.bashrc
RUN echo -e "\"\e[B\": history-search-forward" >> /etc/skel/.bashrc

CMD ["/bin/bash"]