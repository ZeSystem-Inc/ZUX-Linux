#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ZUX_ZIP_URL "https://github.com/ZeSystem-Inc/ZUX-Linux/releases/download/1.0/ZUX.zip"

void clear_screen() {
    printf("\033[H\033[J");
    fflush(stdout);
}

int main() {
    char disk[64] = "/dev/sda";
    char command[512];

    clear_screen();
    printf("==================================================\n");
    printf("               ZUX OS AUTOMATED SETUP             \n");
    printf("==================================================\n\n");

    printf("[i] Sistem diski tespiti yapiliyor...\n");
    
    if (access("/dev/sda", F_OK) == 0) {
        strcpy(disk, "/dev/sda");
    } else if (access("/dev/nvme0n1", F_OK) == 0) {
        strcpy(disk, "/dev/nvme0n1");
    } else if (access("/dev/vda", F_OK) == 0) {
        strcpy(disk, "/dev/vda");
    }

    printf("[i] Hedef Disk: %s\n\n", disk);
    printf("Kuruluma baslamak icin ENTER tusuna basin (Iptal: CTRL+C)...\n");
    fflush(stdout);
    getchar();

    clear_screen();
    printf("[1/4] Disk temizleniyor ve formatlaniyor (%s)...\n", disk);
    fflush(stdout);
    snprintf(command, sizeof(command), "mkfs.ext4 -F %s >/dev/null 2>&1", disk);
    system(command);

    printf("[2/4] Dizin baglaniyor ve ZUX.zip indiriliyor...\n");
    fflush(stdout);
    system("mkdir -p /mnt/target");
    snprintf(command, sizeof(command), "mount %s /mnt/target >/dev/null 2>&1", disk);
    system(command);

    snprintf(command, sizeof(command), "wget -q --show-progress -O /tmp/ZUX.zip %s", ZUX_ZIP_URL);
    if (system(command) != 0) {
        printf("\n[HATA] ZUX.zip indirilemedi! Baglantiyi kontrol edin.\n");
        return 1;
    }

    printf("[3/4] ZUX OS dosyalari kuruluyor...\n");
    fflush(stdout);
    system("unzip -q -o /tmp/ZUX.zip -d /mnt/target/");

    printf("[4/4] Önyukleyici (GRUB) yapilandiriliyor...\n");
    fflush(stdout);
    snprintf(command, sizeof(command), "grub-install --boot-directory=/mnt/target/boot %s >/dev/null 2>&1", disk);
    system(command);

    printf("\n==================================================\n");
    printf(" TEBRIKLER! ZUX OS Kurulumu Tamamlandi.\n");
    printf(" Sistemi yeniden baslatmak icin ENTER'a basabilirsiniz.\n");
    printf("==================================================\n");
    fflush(stdout);
    getchar();

    system("reboot");
    return 0;
}
