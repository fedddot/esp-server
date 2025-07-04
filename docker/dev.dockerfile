FROM espressif/idf:latest

RUN apt-get update
RUN apt-get install -y lsb-release software-properties-common gnupg
RUN apt-get install -y bash-completion
RUN apt-get install -y protobuf-compiler
RUN apt-get install -y locales

ENV MCU_SERVER_PATH=/usr/app/mcu_server
RUN git clone -b main https://github.com/fedddot/mcu_server.git ${MCU_SERVER_PATH}

WORKDIR /usr/app/llvm
RUN wget https://apt.llvm.org/llvm.sh
RUN chmod +x llvm.sh
RUN ./llvm.sh 20
ENV PATH=${PATH}:/usr/lib/llvm-20/bin

ENV SHELL=/bin/bash
RUN echo 'bind "\"\e[A\": history-search-backward"' >> /etc/skel/.bashrc
RUN echo 'bind "\"\e[B\": history-search-forward"' >> /etc/skel/.bashrc
RUN echo 'get_idf 1>/dev/null' >> /etc/skel/.bashrc
RUN echo 'source /etc/skel/.bashrc' >> /root/.bashrc

RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8
ENV LANG="en_US.UTF-8" 

RUN 

CMD ["/bin/bash"]