FROM esp-base:latest

RUN apt-get update
RUN apt-get install -y gcc g++ cmake