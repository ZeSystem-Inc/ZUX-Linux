#!/bin/bin/sh

echo "[ZUX] Ağ ve kurulum ortamı hazırlanıyor..."

ifconfig lo 127.0.0.1 up
udhcpc -i eth0 >/dev/null 2>&1 || true

echo "[ZUX] Yükleyici başlatılıyor..."
echo ""

if [ -f /installer/zux-installer.py ]; then
    python3 /installer/zux-installer.py
else
    echo "[ZUX] zux-installer.py bulunamadı."
fi

exec /bin/sh
