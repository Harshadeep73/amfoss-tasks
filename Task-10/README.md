##Task 10

This task was the most fun. I initially wanted to write a poem but eventually decided to build a
custom grub theme based on one-piece's nations, wano.

So, I just picked up a background I felt was suitable and made this custom grub menu.
The icons though are restricted to 

1.Windows
2.Debian (mine)
3.Arch
4.Fedora
5.Void Linux
6.Endeavour
7.Ubuntu

Note: These icons may fail to load because grub has them mapped to different classes

The process to use this:

1.Clone this part of the repo (using sparse-checkout)

2.Then, check if you have the grub/themes directory: ls -la /boot/grub/themes

3.If not, create it: sudo mkdir -p /boot/grub/themes

4.Then copy this directory (wano) into it: sudo cp -r path/wano /boot/grub/themes/

5.Then point grub (make grub use your imported theme):

sudo sed -i '/^GRUB_THEME=/d' /etc/default/grub
echo 'GRUB_THEME="/boot/grub/themes/wano/theme.txt"' | sudo tee -a /etc/default/grub
echo 'GRUB_GFXMODE=1920x1080' | sudo tee -a /etc/default/grub

6.disable your distro's default theme script if its overriding:

for debian, you'd do: sudo chmod -x /etc/grub.d/05_debian_theme

7.Then, just reload your grub: sudo update-grub

8.Then, Reboot: sudo reboot
 
