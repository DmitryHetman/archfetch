# archfetch
Screenshot information tool for Arch GNU/Linux, written in C, it is fork of gentoofetch.
![archfetch](https://user-images.githubusercontent.com/18743742/61969548-5244b580-afca-11e9-93fe-3d47501661d6.png)
It contains ASCII characrers, so no Unicode required.
To dowload archfetch type in your terminal:
# git clone -b master --depth 1 https://github.com/DmitryHetman/archfetch
GPU detection still doesn't work, you can just edit source code to 
substitute Intel HD with your GPU model:
# $ sandy gentoofetch.c
To run archfetch you must compile and install it:
# $ make
# (root) # make install
