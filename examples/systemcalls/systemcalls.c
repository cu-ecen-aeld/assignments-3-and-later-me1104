#include "systemcalls.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "fcntl.h"
#include "errno.h"
#include "sys/wait.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
   int ret = system(cmd);

   if (WIFEXITED(ret))
   {
	   return true;
   }
   
    return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    int pid;
    int ret, status;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    fflush(stdout);
    pid = fork();

    if (pid == 0) {
	execv(command[0], command);
	_exit(1); //execv failed
    }

    if ((ret = waitpid(pid, &status, 0)) == -1) {
	va_end(args);
	return false;
    }

    if ( WIFEXITED(status) == 0) return false; //child error
 
    //check for status code (shifted 8bit left -> check with WEXITSTATUS)
    //this is only valid if WIFEXITED() is true !
    int exit_code = WEXITSTATUS(status);
    if (exit_code != 0) {
	    va_end(args);
	    return false;
    }

    va_end(args);
    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i,fd, pid;
    int ret, status;
    bool result = true;

    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    fd = open(outputfile, O_TRUNC|O_CREAT|O_WRONLY, 0644);

    if (fd == -1) {
	result = false;
	goto endmark;
    } 

    fflush(stdout);
    pid = fork();

    if (pid == 0) {
	    ret = dup2(fd,1);
	    close(fd);
	    execv(command[0], command);
	    _exit(1); //execv failed
    }

    close(fd);

    if ((ret = waitpid(pid, &status, 0)) == -1) {
	result = false;	
	goto endmark;
    }

    if ( (WIFEXITED(status) == 0) )  {
	 result = false;
	 goto endmark;
    }

    int exit_code = WEXITSTATUS(status);
    if (exit_code != 0) {
	    result = false;
    }

 endmark:
    va_end(args);
    return (result);
}
