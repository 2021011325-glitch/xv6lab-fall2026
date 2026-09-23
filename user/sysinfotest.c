#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

int
main(void)
{
  struct sysinfo info;

  if (sysinfo(&info) < 0) {
    printf("sysinfo failed\n");
    exit(1);
  }

  printf("freepages = %lu pages\n", info.freepages);
  printf("nproc     = %lu\n", info.nproc);

  int p[2];
  char ch;

  if (pipe(p) < 0) {
    printf("pipe failed\n");
    exit(1);
  }

  int pid = fork();

  if (pid < 0) {
    printf("fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    close(p[1]);
    read(p[0], &ch, 1);
    close(p[0]);
    exit(0);
  }

  close(p[0]);

  if (sysinfo(&info) < 0) {
    printf("sysinfo failed\n");
    exit(1);
  }

  printf("after fork nproc = %lu\n", info.nproc);

  write(p[1], "x", 1);
  close(p[1]);

  wait(0);

  if (sysinfo(&info) < 0) {
    printf("sysinfo failed\n");
    exit(1);
  }

  printf("after wait nproc = %lu\n", info.nproc);

  exit(0);
}