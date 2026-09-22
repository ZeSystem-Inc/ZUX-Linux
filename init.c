#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    setenv("PATH", "/bin:/sbin:/usr/bin:/usr/sbin", 1);

    mkdir("/proc", 0755);
    mkdir("/sys", 0755);
    mkdir("/dev", 0755);

    mount("proc", "/proc", "proc", 0, NULL);
    mount("sysfs", "/sys", "sysfs", 0, NULL);
    mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);

    system("echo 1 > /proc/sys/kernel/printk");

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
    printf("[*] Arch-style donanim taramasi baslatiliyor...\n");
    fflush(stdout);

    system("depmod -a 2>/dev/null");

    system("echo /sbin/mdev > /proc/sys/kernel/hotplug");
    system("mdev -s 2>/dev/null");

    system("find /sys/bus/pci/devices /sys/bus/usb/devices -name modalias 2>/dev/null | xargs -r modprobe -a -q 2>/dev/null");

    sleep(1);
    system("mdev -s 2>/dev/null");

    system("ip link set lo up 2>/dev/null");

    system("for iface in $(ls /sys/class/net/ 2>/dev/null | grep -v lo); do "
           "  ip link set $iface up 2>/dev/null; "
           "  echo \"[*] $iface uyarildi, link (carrier) bekleniyor...\"; "
           "  for i in 1 2 3 4 5; do "
           "    if [ \"$(cat /sys/class/net/$iface/carrier 2>/dev/null)\" = \"1\" ]; then "
           "      echo \"[+] $iface baglantisi hazir!\"; "
           "      break; "
           "    fi; "
           "    sleep 1; "
           "  done; "
           "  echo \"[*] $iface uzerinden DHCP istegi gonderiliyor...\"; "
           "  udhcpc -i $iface -n -t 10 -T 3 -q -s /usr/share/udhcpc/default.script; "
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
