
/*
  This file is part of the KDE libraries
  Copyright (c) 1999 Waldo Bastian <bastian@kde.org>
            (c) 1999 Mario Weilguni <mweilguni@sime.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License version 2 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include "ltdl.h"

#include "klauncher_cmds.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>

#include <setproctitle.h>

#define MAX_ARGLENGTH 128*1024

int waitForPid;

/* Group data */
struct {
  int maxname;
  int fd[2];
  int launcher[2]; /* socket pair for launcher communication */
  int deadpipe[2]; /* pipe used to detect dead children */
  int wrapper; /* socket for wrapper communication */
  char result;
  pid_t fork;
  pid_t launcher_pid;
  int n;
  lt_dlhandle handle;
  lt_ptr_t sym;
  char **argv;
  int (*func)(int, char *[]);
  int (*launcher_func)(int);
} d;

char cmdLine[MAX_ARGLENGTH];


/*
 * Close fd's which are only usefull for the parent process.
 * Restore default signal handlers.
 */
void close_fds()
{
   close(d.deadpipe[0]);
   close(d.deadpipe[1]);
   if (d.launcher_pid)
   {
      close(d.launcher[0]);
      close(d.launcher[1]);
   }
   if (d.wrapper)
   {
      close(d.wrapper);
   }

   signal(SIGCHLD, SIG_DFL);
   signal(SIGPIPE, SIG_DFL);
}

pid_t launch(int argc, const char *_name, const char *args)
{
  int l;
  int launcher = 0;
  char *cmd;
  char *name;

  if (strcmp(_name, "klauncher") == 0)
  {
     /* klauncher is launched in a special way:
      * instead of calling 'main(argc, argv)',
      * we call 'start_launcher(comm_socket)'.
      * The other end of the socket is d.launcher[0].
      */
     if (0 > socketpair(AF_UNIX, SOCK_STREAM, 0, d.launcher))
     {
        perror("kinit: scoketpair() failed!\n");
        exit(255);
     }
     launcher = 1;
  }

  if (_name[0] != '/')
  {
     /* Relative name without 'lib' and '.la' */
     strcpy(cmdLine, _name);
     name = cmdLine;
     cmd = strcpy(name + strlen(name) + 1, "lib");
     strncat(cmd, name, MAX_ARGLENGTH - 10);
     strcat(cmd, ".la");
  }
  else
  {
     /* Absolute path including 'lib' and '.la' */
     const char *start;
     char *p;
     const char *cp;
     cp = _name+strlen(_name)-1;
     while((cp >= _name) && (*cp != '/'))
        cp--;
     if (*cp++ != '/') return -1;
     if (*cp++ != 'l') return -1;
     if (*cp++ != 'i') return -1;
     if (*cp++ != 'b') return -1;
     start = cp;
    
     strcpy(cmdLine, start);
     p = cmdLine+strlen(cmdLine)-1;
     if (*p-- != 'a') return -1;
     if (*p-- != 'l') return -1;
     if (*p != '.') return -1;
     *p = 0;
     cmd = strcpy(cmdLine+strlen(cmdLine)+1, _name);
     name = cmdLine; 
  }
  l = strlen(cmd);
  if (!args)
  {
    argc = 1;
  }

  if (0 > pipe(d.fd))
  {
     perror("kinit: pipe() failed!\n");
     exit(255);
  }
  
  d.fork = fork();
  switch(d.fork) {
  case -1:
     perror("kinit: fork() failed!\n");
     exit(255);
     break;
  case 0:
     /** Child **/
     close(d.fd[0]);
     close_fds();
     
     setsid(); 

fprintf(stderr, "arg[0] = %s\n", name);

     /** Give the process a new name **/
     kinit_setproctitle( "%s", name );
         
     d.argv = (char **) malloc(sizeof(char *) * argc);
     d.argv[0] = name;
     for ( l = 1;  l < argc; l++)
     {
        d.argv[l] = (char *) args;
fprintf(stderr, "arg[%d] = %s (%p)\n", l, args, args);
        while(*args != 0) args++;
        args++; 
     }

     printf("Opening \"%s\"\n", cmd);
     d.handle = lt_dlopen( cmd );
     if (!d.handle )
     {
        fprintf(stderr, "Could not dlopen library: %s\n", lt_dlerror());        
        d.result = 1; // Error
        write(d.fd[1], &d.result, 1);
        close(d.fd[1]);
        exit(255);
     }
     
     if (!launcher)
     {
        d.sym = lt_dlsym( d.handle, "main");
        if (!d.sym )
        {
           fprintf(stderr, "Could not find main: %s\n", lt_dlerror());        
           d.result = 0; // Error
           write(d.fd[1], &d.result, 1);
           close(d.fd[1]);
           exit(255);
        }
     
        d.result = 0; // Success
        write(d.fd[1], &d.result, 1);
        close(d.fd[1]);
 
        d.func = (int (*)(int, char *[])) d.sym;

        exit( d.func( argc, d.argv)); /* Launch! */
     }
     else
     {
fprintf(stderr, "Starting klauncher.\n"); 
        d.sym = lt_dlsym( d.handle, "start_launcher");
        if (!d.sym )
        {
           fprintf(stderr, "Could not find start_launcher: %s\n", lt_dlerror());        
           d.result = 0; // Error
           write(d.fd[1], &d.result, 1);
           close(d.fd[1]);
           exit(255);
        }
     
        d.result = 0; // Success
        write(d.fd[1], &d.result, 1);
        close(d.fd[1]);
 
        d.launcher_func = (int (*)(int)) d.sym;

        exit( d.launcher_func( d.launcher[1] )); /* Launch! */
     }
 
     break;
  default:
     /** Parent **/
     close(d.fd[1]);
     if (launcher) 
     {
        d.launcher_pid = d.fork;
     }
     for(;;)
     {
       d.n = read(d.fd[0], &d.result, 1);
       if (d.n == 1) break;
       if (d.n == 0)
       {
          perror("kinit: Pipe closed unexpected.\n");
          exit(255);
       }
       if (errno != EINTR)
       {
          perror("kinit: Error reading from pipe.\n");
          exit(255);
       }
     }
     close(d.fd[0]);
     if (launcher && (d.result == 0)) 
     {
        // Trader launched successfull
        d.launcher_pid = d.fork;
     }
  }
  return d.fork;
}

void sig_child_handler(int) 
{
   /*
    * Write into the pipe of death.
    * This way we are sure that we return from the select()
    *
    * A signal itself causes select to return as well, but
    * this creates a race-condition in case the signal arrives
    * just before we enter the select.
    */
   char c = 0;
   write(d.deadpipe[1], &c, 1);
}

void init_signals()
{
  struct sigaction act;
  long options;
   
  if (pipe(d.deadpipe) != 0)
  {
     perror("Aborting. Can't create pipe: ");
     exit(255);
  }
  
  options = fcntl(d.deadpipe[0], F_GETFL);
  if (options == -1)
  {
     perror("Aborting. Can't make pipe non-blocking: ");
     exit(255);
  }

  if (fcntl(d.deadpipe[0], F_SETFL, options | O_NONBLOCK) == -1)
  {
     perror("Aborting. Can't make pipe non-blocking: ");
     exit(255);
  }

  /*
   * A SIGCHLD handler is installed which sends a byte into the
   * pipe of death. This is to ensure that a dying child causes
   * an exit from select().
   */
  act.sa_handler=sig_child_handler;
  sigemptyset(&(act.sa_mask));
  sigaddset(&(act.sa_mask), SIGCHLD);
  act.sa_flags = SA_NOCLDSTOP;

  // CC: take care of SunOS which automatically restarts interrupted system
  // calls (and thus does not have SA_RESTART)

#ifdef SA_RESTART
  act.sa_flags |= SA_RESTART;
#endif
  sigaction( SIGCHLD, &act, 0L); 

  act.sa_handler=SIG_IGN;
  sigemptyset(&(act.sa_mask));
  sigaddset(&(act.sa_mask), SIGPIPE);
  act.sa_flags = 0;
  sigaction( SIGPIPE, &act, 0L);
}

void init_kinit_socket()
{
  struct sockaddr_un sa;
  socklen_t socklen;
  long options;
  char *sock_file = cmdLine;
  char *home_dir = getenv("HOME");
  if (!home_dir || !home_dir[0])
  {
     fprintf(stderr, "Aborting. $HOME not set!");
     exit(255);
  }   
  chdir(home_dir);
  if (strlen(home_dir) > (MAX_ARGLENGTH -100))
  {
     fprintf(stderr, "Aborting. Home directory path too long!");
     exit(255);
  }
  strcpy(sock_file, home_dir);
  /** Strip trailing '/' **/
  if ( sock_file[strlen(sock_file)-1] == '/')
     sock_file[strlen(sock_file)-1] = 0;
  
  strcat(sock_file, "/.kinit-");
  if (gethostname(sock_file+strlen(sock_file), MAX_ARGLENGTH - strlen(sock_file) - 1) != 0)
  {
     perror("Aborting. Could not determine hostname: ");
     exit(255);
  }

  if (strlen(sock_file) >= sizeof(sa.sun_path))
  {
     fprintf(stderr, "Aborting. Path of socketfile exceeds UNIX_PATH_MAX.\n");
     exit(255);
  }

  /** Delete any stale socket file **/
  unlink(sock_file);

  /** create socket **/
  d.wrapper = socket(PF_UNIX, SOCK_STREAM, 0);
  if (d.wrapper < 0)
  {
     perror("Aborting. socket() failed: ");
     exit(255);
  }

  options = fcntl(d.wrapper, F_GETFL);
  if (options == -1)
  {
     perror("Aborting. Can't make scoket non-blocking: ");
     exit(255);
  }

  if (fcntl(d.wrapper, F_SETFL, options | O_NONBLOCK) == -1)
  {
     perror("Aborting. Can't make scoket non-blocking: ");
     exit(255);
  }

  /** bind it **/
  sa.sun_family = AF_UNIX;
  strcpy(sa.sun_path, sock_file);
  socklen = sizeof(sa);
  if(bind(d.wrapper, (struct sockaddr *)&sa, socklen) != 0) 
  {
     perror("Aborting. bind() failed: ");
     close(d.wrapper);
     exit(255);
  }

  /** set permissions **/
  if (chmod(sock_file, 0600) != 0)
  {
     perror("Aborting. Can't set permissions on socket: ");
     close(d.wrapper);
     exit(255);
  }  

  if(listen(d.wrapper, SOMAXCONN) < 0) 
  {
     perror("Aborting. listen() failed: ");
     close(d.wrapper);
     exit(255);
  }
}

/*
 * Read 'len' bytes from 'sock' into buffer.
 * returns 0 on success, -1 on failure.
 */
int read_socket(int sock, char *buffer, int len)
{
  ssize_t result;
  int bytes_left = len;
  while ( bytes_left > 0)
  {
     result = read(sock, buffer, bytes_left);
     if (result > 0)
     {
        buffer += result;
        bytes_left -= result;
     }
     else if (result == 0)
        return -1;
     else if ((result == -1) && (errno != EINTR))
        return -1;
  }
  return 0;
}

void WaitPid( pid_t waitForPid)
{
  int result;
  while(1)
  {
    result = waitpid(waitForPid, 0, 0);
    if ((result == -1) && (errno == ECHILD))
       return;
  }
}

void kill_launcher()
{
/*   pid_t pid; */
   /* This is bad. Best thing we can do is to kill the launcher. */
   fprintf(stderr, "kinit: Communication error with launcher. Killing launcher!\n");
   if (d.launcher_pid)
   {
     close(d.launcher[0]);
     close(d.launcher[1]);
    
     kill(d.launcher_pid, SIGTERM);
   }
   d.launcher_pid = 0;
   return;
}

void handle_launcher_request()
{
   klauncher_header request_header;
   char *request_data;
   int result = read_socket(d.launcher[0], (char *) &request_header, sizeof(request_header));
   if (result != 0)
   {
      kill_launcher();
      return;
   }
   
   request_data = (char *) malloc(request_header.arg_length);

   result = read_socket(d.launcher[0], request_data, request_header.arg_length);
   if (result != 0)
   {
      kill_launcher();
      free(request_data);
      return;
   }

   if (request_header.cmd == LAUNCHER_EXEC)
   {
      char *name;
      char *args;
      pid_t pid;
      klauncher_header response_header;
      long response_data;
      int argc;
      argc = *((long *) request_data);
      name = request_data + sizeof(long);
      args = name + strlen(name) + 1;

      printf("KInit: args = %d arg_length = %ld\n", argc, request_header.arg_length);

      printf("KInit: Got EXEC '%s' from klauncher \n", name);
      {
         int i = 1;
         char *arg_n;
         arg_n = args;
         while (i < argc)
         {
printf("KInit: argc[%d] = '%s'\n", i, arg_n);
           arg_n = arg_n + strlen(arg_n) + 1;
           i++;
         }   
         if ((arg_n - request_data) != request_header.arg_length)
         {
           fprintf(stderr, "kinit: EXEC request has invalid format.\n"); 
           free(request_data);
           return;
         }
      }
      pid = launch(argc, name, args);
      
      if (pid && (d.result == 0))
      {
         response_header.cmd = LAUNCHER_OK;
         response_header.arg_length = sizeof(response_data);
         response_data = pid;
         write(d.launcher[0], &response_header, sizeof(response_header));
         write(d.launcher[0], &response_data, response_header.arg_length);
      }
      else
      {
         response_header.cmd = LAUNCHER_ERROR;
         response_header.arg_length = 0;
         write(d.launcher[0], &response_header, sizeof(response_header));
      }
   }   
   else if (request_header.cmd == LAUNCHER_SETENV)
   {
      char *env_name;
      char *env_value;
      env_name = request_data;
      env_value = env_name + strlen(env_name) + 1;

      printf("Got SETENV '%s=%s' from klauncher \n", env_name, env_value);

      if ( request_header.arg_length != 
          (int) (strlen(env_name) + strlen(env_value) + 2))
      {
         fprintf(stderr, "kinit: SETENV request has invalid format.\n"); 
         free(request_data);
         return;
      }
      setenv( env_name, env_value, 1);
   }
   free(request_data);
}

void handle_requests()
{
   int max_sock = d.wrapper;
   if (d.launcher_pid && (d.launcher[0] > max_sock))
      max_sock = d.launcher[0];
   max_sock++;

   while(1)
   {
      fd_set rd_set;
      fd_set wr_set;
      fd_set e_set;
      int result;
      pid_t exit_pid;
      char c;

      /* Flush the pipe of death */
      while( read(d.deadpipe[0], &c, 1) == 1);

      /* Handle dying children */
      do {
        exit_pid = waitpid(-1, 0, WNOHANG);
        if (exit_pid > 0)
        {
           fprintf(stderr, "kinit: PID %d terminated.\n", exit_pid);
           if (d.launcher_pid)
           {
              klauncher_header request_header;
              long request_data[2];
              request_header.cmd = LAUNCHER_DIED;
              request_header.arg_length = sizeof(long) * 2;
              request_data[0] = exit_pid;
              request_data[1] = 0; /* not implemented yet */
              write(d.launcher[0], &request_header, sizeof(request_header));
              write(d.launcher[0], request_data, request_header.arg_length);
           }
        }
      }
      while( exit_pid > 0);

      FD_ZERO(&rd_set);
      FD_ZERO(&wr_set);
      FD_ZERO(&e_set);

      if (d.launcher_pid)
      {
         FD_SET(d.launcher[0], &rd_set);
      }
      FD_SET(d.wrapper, &rd_set);
      FD_SET(d.deadpipe[0], &rd_set);

      printf("Entering select...\n");
      result = select(max_sock, &rd_set, &wr_set, &e_set, 0);
      printf("Select done...\n");
      

      /* Handle wrapper request */
      if ((result > 0) && (FD_ISSET(d.wrapper, &rd_set)))
      {
         struct sockaddr_un client;
         socklen_t sClient = sizeof(client);
         int sock = accept(d.wrapper, (struct sockaddr *)&client, &sClient);
         if (sock >= 0)
         {
            if (fork() == 0)
            { 
                close_fds(); 
                /* launchFromSocket(sock);*/
                exit(255); /* Should not come here. */
            }
            close(sock);
         }
      }

      /* Handle launcher request */
      if ((result > 0) && (d.launcher_pid) && (FD_ISSET(d.launcher[0], &rd_set)))
      {
         handle_launcher_request();
      }

   }
}


int main(int argc, char **argv, char **envp)
{
   int i;
   pid_t pid;
   int launch_dcop = 1;
   int launch_klauncher = 1;
   int keep_running = 1;

   /** Save arguments first... **/
   d.argv = (char **) malloc( sizeof(char *) * argc);
   for(i = 0; i < argc; i++) 
   {
      d.argv[i] = strcpy((char*)malloc(strlen(argv[i])+1), argv[i]);
      if (strcmp(d.argv[i], "--no-dcop") == 0)
         launch_dcop = 0;
      if (strcmp(d.argv[i], "--no-klauncher") == 0)
         launch_klauncher = 0;
      if (strcmp(d.argv[i], "--exit") == 0)
         keep_running = 0;
   }
   
   /** Prepare to change process name **/
   kinit_initsetproctitle(argc, argv, envp);  
   kinit_setproctitle("Starting up...");

   unsetenv("LD_BIND_NOW");

   d.maxname = strlen(argv[0]);
   d.launcher_pid = 0;
   d.wrapper = 0;
   init_signals();

   printf("Pre Launcher, pid = %d\n", getpid());

   if (launch_dcop)
   {
      pid = launch( 1, "dcopserver", 0 );
      printf("DCOPServer: pid = %d result = %d\n", pid, d.result);
      WaitPid(pid); /* Wait for dcopserver to fork() */
   }

   if (launch_klauncher)
   {
      pid = launch( 1, "klauncher", 0 );
      printf("KLauncher: pid = %d result = %d\n", pid, d.result);
   }

   for(i = 1; i < argc; i++)
   {
      if (d.argv[i][0] == '+')
      {
         pid = launch( 1, d.argv[i]+1, 0);
         printf("Launched: %s, pid = %d result = %d\n", d.argv[i]+1, pid, d.result);
         WaitPid(pid);
      }
      else if (d.argv[i][0] == '-')
      {
         // Ignore
      }
      else
      {
         pid = launch( 1, d.argv[i], 0 );
         printf("Launched: %s, pid = %d result = %d\n", d.argv[i], pid, d.result);
      }
   }

   /** Free arguments **/
   for(i = 0; i < argc; i++) 
   {
      free(d.argv[i]);
   }
   free (d.argv);

   kinit_setproctitle("Running...");

   if (!keep_running)
      exit(0);

   /*
    * Create ~/.kinit-<hostname> socket for incoming wrapper
    * requests.
    */
   init_kinit_socket();

   if (fork() > 0) // Go into background
      exit(0);

   handle_requests();
   
   return 0;
}

