#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ZUX_ZIP_URL "https://github.com/ZeSystem-Inc/ZUX-Linux/releases/download/1.0/ZUX.zip"

static int run_cmd(const char *cmd) {
    return system(cmd);
}

void clear_screen(void) {
    printf("\033[H\033[J");
    fflush(stdout);
}

int check_ethernet(void) {
    return (run_cmd("ping -c 1 8.8.8.8 >/dev/null 2>&1") == 0);
}

void configure_wifi(void) {
    char ssid[64] = {0};
    char pass[64] = {0};
    char cmd[256];

    printf("\n[!] Kablolu ag baglantisi bulunamadi.\n");
    printf("[*] Wi-Fi Kurulumu:\n");
    printf("--------------------------------------------------\n");
    printf("Wi-Fi Ag Adi (SSID): ");
    fflush(stdout);
    if (scanf("%63s", ssid) != 1) return;

    printf("Wi-Fi Sifresi: ");
    fflush(stdout);
    if (scanf("%63s", pass) != 1) return;

    printf("\n[*] Wi-Fi agina baglaniliyor...\n");
    snprintf(cmd, sizeof(cmd), "wpa_passphrase \"%s\" \"%s\" > /etc/wpa_supplicant.conf", ssid, pass);
    run_cmd(cmd);

    run_cmd("wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf >/dev/null 2>&1");
    run_cmd("udhcpc -i wlan0 >/dev/null 2>&1");
    sleep(2);
}

int main(void) {
    char disk[64] = "/dev/sda";
    char command[512];

    clear_screen();
    printf("==================================================\n");
    printf("               ZUX OS AUTOMATED SETUP             \n");
    printf("==================================================\n\n");

    printf("[1/5] Ag baglantisi kontrol ediliyor...\n");
    
    run_cmd("udhcpc -i eth0 >/dev/null 2>&1 || udhcpc -i enp0s3 >/dev/null 2>&1 || true");

    if (!check_ethernet()) {
        configure_wifi();
        if (!check_ethernet()) {
            printf("\n[HATA] Internet baglantisi saglanamadi! Kurulum iptal ediliyor.\n");
            return 1;
        }
    }
    printf("[+] Ag baglantisi basarili!\n\n");

    printf("[2/5] Sistem diski tespiti yapiliyor...\n");
    if (access("/dev/sda", F_OK) == 0) {
        strcpy(disk, "/dev/sda");
    } else if (access("/dev/nvme0n1", F_OK) == 0) {
        strcpy(disk, "/dev/nvme0n1");
    } else if (access("/dev/vda", F_OK) == 0) {
        strcpy(disk, "/dev/vda");
    }

    printf("[i] Hedef Disk: %s\n\n", disk);
    printf("Kuruluma devam etmek icin ENTER tusuna basin (Iptal: CTRL+C)...");
    fflush(stdout);
    if (getchar() == EOF) {}
    if (getchar() == EOF) {}

    clear_screen();
    printf("[3/5] Disk temizleniyor ve formatlaniyor (%s)...\n", disk);
    fflush(stdout);
    snprintf(command, sizeof(command), "mkfs.ext4 -F %s >/dev/null 2>&1", disk);
    run_cmd(command);

    printf("[4/5] Dizin baglaniyor ve ZUX.zip indiriliyor...\n");
    fflush(stdout);
    run_cmd("mkdir -p /mnt/target");
    snprintf(command, sizeof(command), "mount %s /mnt/target >/dev/null 2>&1", disk);
    run_cmd(command);

    snprintf(command, sizeof(command), "wget -q --show-progress -O /tmp/ZUX.zip %s", ZUX_ZIP_URL);
    if (run_cmd(command) != 0) {
        printf("\n[HATA] ZUX.zip indirilemedi! Baglantiyi veya URL'yi kontrol edin.\n");
        return 1;
    }

    printf("[5/5] ZUX OS kuruluyor ve GRUB yapilandirilization...\n");
    fflush(stdout);
    run_cmd("unzip -q -o /tmp/ZUX.zip -d /mnt/target/");
    snprintf(command, sizeof(command), "grub-install --boot-directory=/mnt/target/boot %s >/dev/null 2>&1", disk);
    run_cmd(command);

    printf("\n==================================================\n");
    printf(" TEBRIKLER! ZUX OS Kurulumu Tamamlandi.\n");
    printf(" Sistemi yeniden baslatmak icin ENTER'a basabilirsiniz.\n");
    printf("==================================================\n");
    fflush(stdout);
    if (getchar() == EOF) {}

    run_cmd("reboot");
    return 0;
}
