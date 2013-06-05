xserver-xorg-video-intel + virtual CRTC
=======================================

These are the package sources for the
[`xserver-xorg-video-intel` package](https://launchpad.net/ubuntu/+source/xserver-xorg-video-intel)
from Ubuntu 13.04 with the
["virtual CRTC" patch](https://github.com/liskin/patches/tree/master/hacks)
applied. Just playing around...

The plan is to always maintain a patched version of the one in the official Ubuntu repositories for 13.04.

Install the package on Ubuntu 13.04 (Raring Ringtail) from the corresponding PPA (as root):

    add-apt-repository ppa:krlmlr/ppa
    apt-get update
    apt-get install xserver-xorg-video-intel
