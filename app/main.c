#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void copy_file(const char *from, const char *to) {
  char cmd[1024];
  snprintf(cmd, sizeof(cmd), "cp %s %s", from, to);
  system(cmd);
}

int main(int argc, char *argv[]) {
  // Disable output buffering
  setbuf(stdout, NULL);

  int pipe_out[2];
  pipe(pipe_out);
  int pipe_err[2];
  pipe(pipe_err);

  char *command = "./sandbox/ex";
  mkdir("./sandbox", 0777);

  copy_file(argv[3], command);

  chroot("./sandbox");
  unshare(CLONE_NEWPID);

  int child_pid = fork();

  if (child_pid == -1) {
    printf("Error forking!");
    return 1;
  }

  if (child_pid == 0) {
    // pipe stdout to parent
    close(pipe_err[0]);
    close(pipe_out[0]);

    dup2(pipe_out[1], STDOUT_FILENO);
    dup2(pipe_err[1], STDERR_FILENO);

    execv(command, &argv[3]);

    close(pipe_out[1]);
    close(pipe_err[1]);

  } else {
    close(pipe_out[1]);
    close(pipe_err[1]);

    char buffer[1024];
    size_t len = 0;

    while ((len = read(pipe_out[0], buffer, sizeof(buffer))) > 0) {
      write(STDOUT_FILENO, buffer, len);
    }

    while ((len = read(pipe_err[0], buffer, sizeof(buffer))) > 0) {
      write(STDERR_FILENO, buffer, len);
    }

    close(pipe_out[0]);
    close(pipe_err[0]);

    int child_status = 0;
    wait(&child_status);
    exit(WEXITSTATUS(child_status));
  }

  return 0;
}
