#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Disable output buffering
  setbuf(stdout, NULL);

  if (argc < 4) {
    fprintf(stderr, "Usage: %s run <image> <command> <arg1> <arg2> ...\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  int stdout_pipe[2];
  int stderr_pipe[2];
  char buffer[4096];

  // Create pipes for stdout and stderr
  if (pipe(stdout_pipe) == -1 || pipe(stderr_pipe) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  char *command = argv[3];
  int child_pid = fork();
  if (child_pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (child_pid == 0) {
    // Child process

    // Close the read-end of the pipes in the child
    close(stdout_pipe[0]);
    close(stderr_pipe[0]);

    // Redirect stdout to the write-end of the stdout pipe
    dup2(stdout_pipe[1], STDOUT_FILENO);
    // Redirect stderr to the write-end of the stderr pipe
    dup2(stderr_pipe[1], STDERR_FILENO);

    // Execute the command
    execv(command, &argv[3]);

    // execv only returns if there is an error
    perror("execv");
    exit(EXIT_FAILURE);
  } else {
    // Parent process

    // Close the write-end of the pipes in the parent
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

    // Wait for child to complete
    wait(NULL);

    // Read from stdout pipe
    int bytes_read = read(stdout_pipe[0], buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
      buffer[bytes_read] = '\0'; // Null-terminate the string
      printf("%s", buffer);
    }

    // Read from stderr pipe
    bytes_read = read(stderr_pipe[0], buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
      buffer[bytes_read] = '\0'; // Null-terminate the string
      fprintf(stderr, "%s", buffer);
    }

    // Close the read-end of the pipes
    close(stdout_pipe[0]);
    close(stderr_pipe[0]);
  }

  return 0;
}
