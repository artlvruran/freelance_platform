FROM archlinux

RUN pacman -Syu --noconfirm
RUN pacman -S sudo --noconfirm
RUN pacman -S base-devel fakeroot jshon expac git wget debugedit --noconfirm

LABEL authors="kirill"

WORKDIR /

COPY . /

ARG psswd=1

RUN useradd --create-home docker
RUN echo "docker:${psswd}" | chpasswd
RUN usermod -aG wheel docker
RUN echo '%wheel ALL=(ALL) NOPASSWD: ALL' > /etc/sudoers.d/wheel

USER docker

RUN sudo chown docker /
RUN sudo chmod 777 .

RUN ./install_paru.sh $psswd

RUN paru -S cppcms --noconfirm
RUN paru -S soci --noconfirm
