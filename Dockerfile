FROM ubuntu:18.04
RUN apt update
RUN apt-get install -y sudo git build-essential libssl-dev libgmp-dev wget python-pip
RUN pip install cmake --upgrade
RUN git clone https://github.com/ladnir/relic.git
WORKDIR /relic
RUN cmake . -DMULTI=PTHREAD
RUN make install 
WORKDIR /home
RUN git clone --recursive https://github.com/osu-crypto/libOTe.git
WORKDIR /home/libOTe/cryptoTools/thirdparty/linux
RUN bash boost.get
WORKDIR /home/libOTe/cryptoTools/thirdparty/linux/boost
RUN sudo ./b2 install
WORKDIR /home/libOTe
RUN cmake . -DENABLE_IKNP=ON -DENABLE_DELTA_IKNP=ON -DENABLE_NP=ON -DENABLE_RELIC=ON
RUN make install
WORKDIR /home
RUN git clone https://github.com/siamumar/test-libOTE
WORKDIR /home/test-libOTE
RUN cmake .
RUN make
WORKDIR /home