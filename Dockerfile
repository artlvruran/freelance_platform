FROM archlinux

WORKDIR /

COPY . /

RUN cat mirrorList >> /etc/pacman.d/mirrorlist


RUN pacman -Syu --noconfirm
RUN pacman -S sudo --noconfirm
RUN pacman -S mesa base-devel fakeroot jshon expac git wget debugedit cmake tk bluez-libs gdb xorg-server-xvfb xterm boost --noconfirm

LABEL authors="kirill"

ARG psswd=1

RUN useradd --create-home docker
RUN echo "docker:${psswd}" | chpasswd
RUN usermod -aG wheel docker
RUN echo '%wheel ALL=(ALL) NOPASSWD: ALL' > /etc/sudoers.d/wheel

USER docker

RUN sudo chown docker /
RUN sudo chmod 777 .

RUN ./install_python2.sh
RUN ./install_cppcms.sh
RUN ./install_soci.sh

EXPOSE 4000

CMD ["run.sh"]