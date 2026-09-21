#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ZUX_ZIP_URL "https://github.com/KULLANICI_ADI/REPO_ADI/releases/latest/download/ZUX.zip"

void clear_screen() {
    printf("\033[H\033[J");
    fflush(stdout);
}

int main() {
    char disk[64] = "/dev/sda";
    char command[512];

    clear_screen();
    printf("==================================================\n");
    printf("             WINDOWS SETUP STYLE - ZUX OS         \n");
    printf("==================================================\n\n");

    printf("[1/3] Mevcut Diskler:\n");
    printf("--------------------------------------------------\n");
    system("lsblk || fdisk -l");
    printf("--------------------------------------------------\n");
    printf("Kurulacak Disk (ornek: /dev/sda): ");
    fflush(stdout);

    if (scanf("%63s", disk) != 1) {
        strcpy(disk, "/dev/sda");
    }

    clear_screen();
    printf("==================================================\n");
    printf("             ZUX OS SETUP WIZARD                  \n");
    printf("==================================================\n\n");
    printf("Hedef Disk: %s\n", disk);
    printf("Paket URL : %s\n\n", ZUX_ZIP_URL);
    printf("Kuruluma baslansin mi? (y/n): ");
    fflush(stdout);

    char confirm[10];
    scanf("%9s", confirm);

    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("\nKurulum iptal edildi.\n");
        return 0;
    }

    clear_screen();
    printf("[1/4] Disk formatlaniyor (%s)...\n", disk);
    fflush(stdout);
    snprintf(command, sizeof(command), "mkfs.ext4 -F %s", disk);
    system(command);

    printf("[2/4] Hedef dizin baglaniyor ve ZUX.zip indiriliyor...\n");
    fflush(stdout);
    system("mkdir -p /mnt/target");
    snprintf(command, sizeof(command), "mount %s /mnt/target", disk);
    system(command);

    snprintf(command, sizeof(command), "wget -O /tmp/ZUX.zip %s", ZUX_ZIP_URL);
    if (system(command) != 0) {
        printf("\nHATA: ZUX.zip indirilemedi!\n");
        return 1;
    }

    printf("[3/4] ZUX OS kuruluyor...\n");
    fflush(stdout);
    system("unzip -q /tmp/ZUX.zip -d /mnt/target/");

    printf("[4/4] Önyukleyici yapılandırılıyor...\n");
    fflush(stdout);
    snprintf(command, sizeof(command), "grub-install --boot-directory=/mnt/target/boot %s", disk);
    system(command);

    printf("\n==================================================\n");
    printf(" TEBRIKLER! ZUX OS Kurulumu Tamamlandi.\n");
    printf("==================================================\n");

    return 0;
}
