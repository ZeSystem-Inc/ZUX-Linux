#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ZUX_ZIP_URL "https://github.com/KULLANICI_ADI/REPO_ADI/releases/latest/download/ZUX.zip"

void clear_screen() {
    printf("\033[H\033[J");
}

int main() {
    char disk[64] = "/dev/sda";
    int desktop_choice = 1;
    char command[512];

    clear_screen();
    printf("==================================================\n");
    printf("             ZUX OS SYSTEM INSTALLER              \n");
    printf("==================================================\n\n");

    printf("[1/3] Kullanilabilir Diskler:\n");
    printf("--------------------------------------------------\n");
    system("lsblk -d -n -o NAME,SIZE || fdisk -l | grep 'Disk /dev/'");
    printf("--------------------------------------------------\n");
    printf("Kurulum yapilacak diski girin [Varsayilan: /dev/sda]: ");
    
    char input_disk[64];
    if (fgets(input_disk, sizeof(input_disk), stdin) && input_disk[0] != '\n') {
        input_disk[strcspn(input_disk, "\n")] = 0;
        strcpy(disk, input_disk);
    }

    clear_screen();
    printf("==================================================\n");
    printf("             ZUX OS SYSTEM INSTALLER              \n");
    printf("==================================================\n\n");
    printf("[2/3] Masaustu Ortami Secimi:\n");
    printf("--------------------------------------------------\n");
    printf("  1) Futuristiks Masaustu (GNOME)\n");
    printf("  2) Standart Masaustu (XFCE)\n");
    printf("--------------------------------------------------\n");
    printf("Seciminiz (1 veya 2) [Varsayilan: 1]: ");

    char choice_buf[10];
    if (fgets(choice_buf, sizeof(choice_buf), stdin) && choice_buf[0] != '\n') {
        desktop_choice = atoi(choice_buf);
    }

    clear_screen();
    printf("==================================================\n");
    printf("             ZUX OS SYSTEM INSTALLER              \n");
    printf("==================================================\n\n");
    printf("[3/3] Kurulum Ozet:\n");
    printf("  - Hedef Disk: %s\n", disk);
    printf("  - Masaustu  : %s\n", (desktop_choice == 2) ? "Standart (XFCE)" : "Futuristiks (GNOME)");
    printf("  - Indirme   : %s\n\n", ZUX_ZIP_URL);

    printf("Devam etmek istiyor musunuz? (e/h): ");
    char confirm[10];
    if (!fgets(confirm, sizeof(confirm), stdin) || (confirm[0] != 'e' && confirm[0] != 'E')) {
        printf("Kurulum iptal edildi.\n");
        return 0;
    }

    printf("\n[1/4] Disk bicimlendiriliyor (%s1)...\n", disk);
    snprintf(command, sizeof(command), "mkfs.ext4 -F %s1 2>/dev/null || mkfs.ext4 -F %s", disk, disk);
    system(command);

    printf("[2/4] Disk baglaniyor ve ZUX.zip indiriliyor...\n");
    system("mkdir -p /mnt/target");
    snprintf(command, sizeof(command), "mount %s1 /mnt/target 2>/dev/null || mount %s /mnt/target", disk, disk);
    system(command);

    snprintf(command, sizeof(command), "wget -O /tmp/ZUX.zip %s", ZUX_ZIP_URL);
    if (system(command) != 0) {
        printf("\nHATA: ZUX.zip indirilemedi! Ag baglantisini kontrol edin.\n");
        return 1;
    }

    printf("[3/4] ZUX OS dosyalari diske cikariliyor...\n");
    system("unzip -q /tmp/ZUX.zip -d /mnt/target/");

    printf("[4/4] Önyukleyici (GRUB) kuruluyor...\n");
    snprintf(command, sizeof(command), "grub-install --boot-directory=/mnt/target/boot %s 2>/dev/null", disk);
    system(command);

    printf("\n==================================================\n");
    printf(" TEBRIKLER! ZUX OS Basariyla Kuruldu!\n");
    printf("==================================================\n");
    printf("Sistemi yeniden baslatabilirsiniz (reboot).\n");

    return 0;
}
