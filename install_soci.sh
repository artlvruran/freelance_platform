#!/bin/bash
sudo git clone https://aur.archlinux.org/soci.git
sudo chmod 777 soci
cd soci
makepkg -sricCf --noconfirm