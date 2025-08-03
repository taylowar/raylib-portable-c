#ifndef CAKE_CMD_H_
#define CAKE_CMD_H_

typedef struct {
    size_t size;
    size_t capacity;
    const char* *es;
} Cake_CMD;

void cake_cmd_print(Cake_CMD cmd);

#define cake_cmd_push(cmd, ...) \
    cake_da_append_many(cmd, (sizeof((const char*[]){__VA_ARGS__})/sizeof(const char*)), ((const char*[]){__VA_ARGS__}))

pid_t cake_cmd_exec(Cake_CMD cmd);
bool cake_cmd_wait(pid_t cpid);
void cake_cmd_reset(Cake_CMD *cmd);

#ifdef CAKE_CMD_IMPLEMENTATION

void cake_cmd_print(Cake_CMD cmd)
{
    printf("+ ");
    for (size_t i=0; i<cmd.size; ++i) {
        printf("%s ", cmd.es[i]);
    }
    printf("\n");
}

pid_t cake_cmd_exec(Cake_CMD cmd)
{
    // TODO: cmd.es is not NULL terminated so you should append `0` at the end
    cake_cmd_push(&cmd, 0);
    pid_t cpid = fork();
    if (cpid < 0) {
        fprintf(stderr, "ERROR: could not fork child: %s\n", strerror(errno));
        exit(1);
    } 
    if (cpid == 0) {
        if (execvp(cmd.es[0], (char * const*)cmd.es) < 0) {
            fprintf(stderr, "ERROR: could not execute child: %s\n", strerror(errno));
            exit(1);
        }
    }
    return cpid;
}

bool cake_cmd_wait(pid_t cpid)
{
    if (cpid == -1) {
        return false;
    }
    for (;;) {
        int wstatus = 0;
        if (waitpid(cpid, &wstatus, 0) < 0) {
            fprintf(stderr, "ERROR: unable to wait on child: %s\n", strerror(errno));
            return false;
        }
        if (WIFEXITED(wstatus)) {
            int exit_code = WEXITSTATUS(wstatus);
            if (exit_code != 0) {
                fprintf(stderr, "ERROR: child exited with code: %d\n", exit_code);
                return false;
            }
            return true;
        }
        if (WIFSIGNALED(wstatus)) {
            fprintf(stderr, "ERROR: child was terminated by a signal: %d\n", WTERMSIG(wstatus));
            return false;
        }
    }
}

void cake_cmd_reset(Cake_CMD *cmd)
{
    cmd->size = 0;
    free(cmd->es);
    cmd->es = NULL;
}

#endif // CAKE_CMD_IMPLEMENTATION
#endif // CAKE_CMD_H_
