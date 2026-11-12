#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    // O_APPEND 保证每次 write 都会自动移动到文件末尾，无需手动 lseek
    int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    char buf[128];
    for (int i = 0; i < 5; i++) {
        // 格式化字符串：序号 + 进程ID
        int len = snprintf(buf, sizeof(buf), "第 %d 次写入，来自进程 PID: %d\n", i, getpid());
        
        char *ptr = buf;
        ssize_t n_left = len;
        
        // 核心：防御性编程，确保必须把 len 个字节全部写进去
        while (n_left > 0) {
            ssize_t n_write = write(fd, ptr, n_left);
            if (n_write == -1) {
                perror("write");
                close(fd);
                exit(1);
            }
            n_left -= n_write;
            ptr += n_write;
        }
    }

    close(fd);
    printf("写入完成，请查看 log.txt\n");
    return 0;
}