#!/bin/bash
psswd=$1
sudo git clone https://aur.archlinux.org/paru-bin.git
sudo chmod 777 paru-bin
cd paru-bin
makepkg -sricCf --noconfirm