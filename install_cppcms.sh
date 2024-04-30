#!/bin/bash
sudo git clone https://aur.archlinux.org/cppcms.git
sudo chmod 777 cppcms
cd cppcms
makepkg -sricCf --noconfirm