//
//  KPROCESSCONTROLLER -- A helper class for KProcess
//
//  version 0.2.2, Aug 31st 1997
//
//  (C) Christian Czezatke
//  e9025461@student.tuwien.ac.at
//

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <stdio.h>

#include "kprocess.h"
#define _MAY_INCLUDE_KPROCESSCONTROLLER_
#include "kprocctrl.h"

#include "kprocctrl.moc"


KProcessController *theKProcessController = NULL;

KProcessController::KProcessController()
{
  struct sigaction act;

  // initialize theKProcessList
  processList = new QList<KProcess>();
  CHECK_PTR(processList);
 
  if (0 > pipe(fd))
	printf(strerror(errno));
  
  if (-1 == fcntl(fd[0], F_SETFL, O_NONBLOCK))
	printf(strerror(errno));

  notifier = new QSocketNotifier(fd[0], QSocketNotifier::Read);
  notifier->setEnabled(TRUE);
  QObject::connect(notifier, SIGNAL(activated(int)),
				   this, SLOT(slotDoHousekeeping(int)));
 		 
  act.sa_handler=theSigCHLDHandler;
  sigemptyset(&(act.sa_mask));
  sigaddset(&(act.sa_mask), SIGCHLD);
  act.sa_flags = SA_NOCLDSTOP;

  // CC: take care of SunOS which automatically restarts interrupted system
  // calls (and thus does not have SA_RESTART)

#ifdef SA_RESTART
  act.sa_flags |= SA_RESTART;
#endif

  sigaction( SIGCHLD, &act, NULL); 
  act.sa_handler=SIG_IGN;
  sigemptyset(&(act.sa_mask));
  sigaddset(&(act.sa_mask), SIGPIPE);
  act.sa_flags = 0;
  sigaction( SIGPIPE, &act, NULL);
}

void KProcessController::theSigCHLDHandler(int )
{
  int status;
  pid_t this_pid;
  int saved_errno;

  saved_errno = errno;
  // since waitpid and write change errno, we have to save it and restore it
  // (Richard Stevens, Advanced programming in the Unix Environment)

  this_pid = waitpid(-1, &status, WNOHANG);
  if (-1 != this_pid) {
    ::write(theKProcessController->fd[1], &this_pid, sizeof(this_pid));
    ::write(theKProcessController->fd[1], &status, sizeof(status));
  }
  errno = saved_errno;
}



void KProcessController::slotDoHousekeeping(int )
{
  KProcess *proc;
  int bytes_read;
  pid_t pid;
  int status;

  bytes_read  = ::read(fd[0], &pid, sizeof(pid_t));
  bytes_read += ::read(fd[0], &status, sizeof(int));

  if (bytes_read != sizeof(int)+sizeof(pid_t))
	fprintf(stderr,"Error: Could not read info from signal handler!\n");
 
  proc = processList->first();

  while (NULL != proc) {
	if (proc->pid == pid) {
	  // process has exited, so do emit the respective events
	  proc->processHasExited(status);
	}
	proc = processList->next();
  }
}


