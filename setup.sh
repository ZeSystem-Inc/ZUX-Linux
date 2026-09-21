#!/bin/sh

clear
echo "=================================================="
echo "         ZUX Linux Grafiksel Kurulum              "
echo "=================================================="
echo "Grafiksel arayüz ve sürücüler hazırlanıyor..."

if [ ! -c /dev/dri/card0 ]; then
    mdev -s 2>/dev/null || udevadm trigger 2>/dev/null
fi

echo "Ağ bağlantısı yapılandırılıyor..."
ip link set lo up
dhcpcd 2>/dev/null || udhcpc -b -i eth0 2>/dev/null

export DISPLAY=:0
export XDG_RUNTIME_DIR=/tmp/runtime-root
mkdir -p -m 0700 "$XDG_RUNTIME_DIR"

export GTK_THEME=Adwaita:dark
export GDK_BACKEND=x11,wayland

if command -v weston >/dev/null 2>&1; then
    echo "Wayland (Weston) başlatılıyor..."
    weston --backend=drm-backend.so --shell=kiosk-shell.so --exec="/usr/bin/python3 /usr/bin/zux-installer.py"
elif command -v xinit >/dev/null 2>&1; then
    echo "X11 (Xorg) başlatılıyor..."
    xinit /usr/bin/python3 /usr/bin/zux-installer.py -- :0 vt1 -nolisten tcp
else
    echo "Hata: Grafik sunucusu (Wayland/X11) bulunamadı!"
    echo "Konsol kabuğuna düşülüyor..."
    exec /bin/sh
fi
