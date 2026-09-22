#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    (void)setenv("PATH", "/bin:/sbin:/usr/bin:/usr/sbin", 1);

    mkdir("/proc", 0755);
    mkdir("/sys", 0755);
    mkdir("/dev", 0755);

    mount("proc", "/proc", "proc", 0, NULL);
    mount("sysfs", "/sys", "sysfs", 0, NULL);
    mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);

    (void)system("echo 1 > /proc/sys/kernel/printk");

    int fd = open("/dev/tty1", O_RDWR);
    if (fd < 0) fd = open("/dev/console", O_RDWR);
    if (fd >= 0) {
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
        if (fd > 2) close(fd);
    }

    printf("\033[H\033[J");
    printf("===============================================\n");
    printf("          ZUX OS Preinstallation Shell         \n");
    printf("===============================================\n\n");
    printf("[*] Donanim ve ag suruculeri yukleniyor...\n");
    fflush(stdout);

    (void)system("depmod -a 2>/dev/null");

    (void)system("echo /sbin/mdev > /proc/sys/kernel/hotplug");
    (void)system("mdev -s 2>/dev/null");

    (void)system("modprobe pci_hotplug 2>/dev/null");
    (void)system("modprobe mii 2>/dev/null");
    (void)system("modprobe ptp 2>/dev/null");
    (void)system("modprobe pps_core 2>/dev/null");

    (void)system("modprobe e1000 2>/dev/null");
    (void)system("modprobe e1000e 2>/dev/null");
    (void)system("modprobe virtio_net 2>/dev/null");
    (void)system("modprobe pcnet32 2>/dev/null");
    (void)system("modprobe r8169 2>/dev/null");
    (void)system("modprobe 8139too 2>/dev/null");

    (void)system("find /sys/bus/pci/devices /sys/bus/usb/devices -name modalias 2>/dev/null | xargs -r modprobe -a -q 2>/dev/null");

    sleep(2);
    (void)system("mdev -s 2>/dev/null");

    (void)system("ip link set lo up 2>/dev/null");

    printf("[*] Ag arabirimleri kontrol ediliyor...\n");
    fflush(stdout);

    (void)system("for iface in $(ls /sys/class/net/ 2>/dev/null | grep -v lo); do "
                 "  ip link set $iface up 2>/dev/null; "
                 "  echo \"[+] Arabirim bulundu: $iface, baglanti bekleniyor...\"; "
                 "  for i in 1 2 3 4 5; do "
                 "    if [ \"$(cat /sys/class/net/$iface/carrier 2>/dev/null)\" = \"1\" ]; then "
                 "      echo \"[+] $iface baglantisi aktif!\"; "
                 "      break; "
                 "    fi; "
                 "    sleep 1; "
                 "  done; "
                 "  echo \"[*] $iface uzerinden DHCP istegi gonderiliyor...\"; "
                 "  udhcpc -i $iface -n -t 8 -T 2 -q -s /usr/share/udhcpc/default.script; "
                 "done");

    if (access("/installer/setup.elf", X_OK) == 0) {
        printf("\n[*] ZUX OS Otomatik Kurulum Baslatiliyor...\n\n");
        fflush(stdout);
        pid_t pid = fork();
        if (pid == 0) {
            execl("/installer/setup.elf", "/installer/setup.elf", NULL);
            _exit(1);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    printf("\n[*] Shell ekranina aktariliyorsunuz...\n\n");
    fflush(stdout);

    while (1) {
        if (access("/bin/busybox", X_OK) == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                execl("/bin/busybox", "busybox", "sh", NULL);
                _exit(1);
            } else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
            }
        }
        sleep(1);
    }
    return 0;
}
