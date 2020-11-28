# Small example of a program, which daemonizes itself

1. Create a new proces with `fork()`.
2. Exit from parent, because daemons should be a child of init process.
3. Create new session for prosess's group in the child process with `setsid()`.
4. Set root catalog for the child process as a current catalog with `chdir("/")`.
5. Close all opened by parent files.
______________________________________________________
Or just call `daemon(3)` instead of these steps.

