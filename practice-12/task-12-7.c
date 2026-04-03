#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("Родитель: PID = %d\n", getpid());
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Дочерний процесс
        printf("Ребёнок: PID = %d, родитель = %d\n", getpid(), getppid());
        sleep(1);
        printf("Ребёнок завершается\n");
    } else if (pid > 0) {
        // Родительский процесс
        printf("Родитель: создан ребёнок с PID = %d\n", pid);
        wait(NULL);
        printf("Родитель: ребёнок завершился\n");
    } else {
        perror("fork");
        return 1;
    }
    
    return 0;
}
