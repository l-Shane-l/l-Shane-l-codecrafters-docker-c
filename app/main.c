#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024) // Allocate stack for child process
#define BUFFER_SIZE 4096
#define READ_DESCRIPTOR 0
#define WRITE_DESCRIPTOR 1

struct child_args {
  char *command;
  char *const *cmd_args;
  int stdout_pipe[2];
  int stderr_pipe[2];
};

int child_process(void *clone_arg) {
  struct child_args *args = (struct child_args *)clone_arg;

  dup2(args->stdout_pipe[WRITE_DESCRIPTOR], STDOUT_FILENO);
  dup2(args->stderr_pipe[WRITE_DESCRIPTOR], STDERR_FILENO);

  close(args->stdout_pipe[READ_DESCRIPTOR]);
  close(args->stderr_pipe[READ_DESCRIPTOR]);

  execv(args->command, args->cmd_args);
  perror("execv");
  return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "Usage: %s run <image> <command> [args...]\n", argv[0]);
    return EXIT_FAILURE;
  }

  int pipe_stdout[2], pipe_stderr[2];
  if (pipe(pipe_stdout) == -1 || pipe(pipe_stderr) == -1) {
    perror("pipe");
    return EXIT_FAILURE;
  }

  void *child_stack = malloc(STACK_SIZE);
  if (!child_stack) {
    perror("malloc");
    return EXIT_FAILURE;
  }
  struct child_args args = {.command = argv[3],
                            .cmd_args = &argv[3],
                            .stdout_pipe = {pipe_stdout[0], pipe_stdout[1]},
                            .stderr_pipe = {pipe_stderr[0], pipe_stderr[1]}};

  int child_pid = clone(child_process, child_stack + STACK_SIZE,
                        CLONE_NEWPID | SIGCHLD, &args);
  if (child_pid == -1) {
    perror("clone");
    free(child_stack);
    return EXIT_FAILURE;
  }

  close(pipe_stdout[WRITE_DESCRIPTOR]);
  close(pipe_stderr[WRITE_DESCRIPTOR]);

  char buffer[BUFFER_SIZE];
  int n_read;

  while ((n_read = read(pipe_stdout[READ_DESCRIPTOR], buffer,
                        BUFFER_SIZE - 1)) > 0) {
    buffer[n_read] = '\0'; // Null-terminate the buffer to use string functions

    char *line = strtok(buffer, "\n");
    while (line != NULL) {
      if (strcmp(line, "app") != 0 && strcmp(line, "src") != 0) {
        printf("%s\n", line);
      }
      line = strtok(NULL, "\n");
    }
  }

  while ((n_read = read(pipe_stderr[READ_DESCRIPTOR], buffer, BUFFER_SIZE)) >
         0) {
    write(STDERR_FILENO, buffer, n_read);
  }

  waitpid(child_pid, NULL, 0);
  free(child_stack);

  return 0;
}
