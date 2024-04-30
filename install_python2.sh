#!/bin/bash
sudo git clone https://aur.archlinux.org/python2.git
sudo chmod 777 python2
cd python2
makepkg -sricCf --noconfirm --skippgp --nocheck