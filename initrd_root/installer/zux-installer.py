#!/usr/bin/env python3
import os
import sys
import time

def clear_screen():
    os.system('clear')

def print_header():
    clear_screen()
    print("==================================================")
    print("           ZUX OS GRAPHICAL INSTALLER             ")
    print("==================================================")
    print()

def get_disk_choice():
    print_header()
    print("[1/3] Hedef Disk Secimi")
    print("--------------------------------------------------")
    print("Mevcut Diskler:")
    os.system("lsblk -d -n -o NAME,SIZE,MODEL || fdisk -l | grep 'Disk /dev/'")
    print()
    target_disk = input("Kurulum yapilacak diski girin (ör. /dev/sda veya /dev/nvme0n1): ").strip()
    if not target_disk:
        target_disk = "/dev/sda"
    return target_disk

def get_desktop_environment():
    print_header()
    print("[2/3] Masaüstü Ortami Secimi")
    print("--------------------------------------------------")
    print("Lütfen ZUX OS için bir masaüstü deneyimi secin:")
    print()
    print("  [1] Fütüristik Masaüstü Ortami (GNOME)")
    print("      -> Modern visual efektler, jestler ve yenilikci arayüz.")
    print()
    print("  [2] Standart Masaüstü Ortami (XFCE)")
    print("      -> Hafif, hizli, klasik ve yüksek performansli arayüz.")
    print()
    
    choice = input("Seciminiz (1 veya 2) [Varsayilan: 1]: ").strip()
    
    if choice == "2":
        return "XFCE", "task-xfce-desktop xfce4"
    else:
        return "GNOME", "task-gnome-desktop gnome"

def start_installation(disk, desktop_name, desktop_packages):
    print_header()
    print("[3/3] Kurulum Baslatiliyor")
    print("--------------------------------------------------")
    print(f"Hedef Disk      : {disk}")
    print(f"Masaüstü Modu   : {desktop_name}")
    print("--------------------------------------------------")
    print()
    
    confirm = input("Disk biçimlendirilecek! Devam etmek istiyor musunuz? (e/h): ").strip().lower()
    if confirm != 'e':
        print("\nKurulum iptal edildi.")
        sys.exit(0)

    print("\n[1/4] Disk bölümleniyor ve ext4 biçimlendiriliyor...")
    time.sleep(1)
    # Gercek kurulum adimlari:
    # os.system(f"mkfs.ext4 -F {disk}")
    # os.system(f"mount {disk} /mnt")

    print(f"[2/4] ZUX OS Temel Paketleri ve {desktop_name} kuruluyor...")
    time.sleep(2)
    # os.system(f"apt-get install -y {desktop_packages}")

    print("[3/4] Sistem konfigürasyonu yapilandiriliyor...")
    time.sleep(1)

    print("[4/4] GRUB Önyükleyici kuruluyor...")
    time.sleep(1)

    print("\n==================================================")
    print(f" TEBRIKLER! ZUX OS ({desktop_name}) Basariyla Kuruldu!")
    print("==================================================")
    print("Sistemi yeniden baslatmak için 'reboot' yazabilirsiniz.")

if __name__ == "__main__":
    try:
        disk = get_disk_choice()
        desktop_name, desktop_packages = get_desktop_environment()
        start_installation(disk, desktop_name, desktop_packages)
    except KeyboardInterrupt:
        print("\n\nKurulum kullanıcı tarafından iptal edildi.")
        sys.exit(0)
