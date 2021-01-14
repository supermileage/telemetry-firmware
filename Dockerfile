FROM ubuntu:20.04

# Install misc build tools
RUN apt-get update \
    && apt-get install -y wget git make libarchive-zip-perl vim-common

# Install ARM gcc toolchain
RUN wget https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q1-update/+download/gcc-arm-none-eabi-5_3-2016q1-20160330-linux.tar.bz2 \
    && bzip2 -d gcc-arm-none-eabi-5_3-2016q1-20160330-linux.tar.bz2 \
    && tar xf gcc-arm-none-eabi-5_3-2016q1-20160330-linux.tar \
    && mv gcc-arm-none-eabi-5_3-2016q1 /usr/local/gcc-arm

# Install 32-bit libraries
RUN dpkg --add-architecture i386 \
    && apt-get update \
    && apt-get install -y libc6:i386 libncurses5:i386 libstdc++6:i386

# We track v1.5.x for now since v2.x seems to not build properly
RUN git clone https://github.com/particle-iot/device-os.git firmware \
    && cd firmware \
    && git checkout release/v1.5.x \
    && git submodule update --init

# Set the environment variable
ENV PATH=$PATH:/usr/local/gcc-arm/bin

WORKDIR firmware/main

ENTRYPOINT ["/bin/bash"]

# Our default command to run, which is to target the electron with the app-dir and target-dir set
CMD ["make", "all", "PLATFORM=electron", "APPDIR=../../app", "TARGET_DIR=../../out"]

