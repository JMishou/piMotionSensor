/*
 * daemonize.c
 * This example daemonizes a process, writes a few log messages,
 * sleeps 20 seconds and terminates afterwards.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>


// Use GPIO Pin 22, which is Pin 3 for wiringPi library
 
#define BUTTON_PIN 3
 
// the event counter 
volatile int currentState = 1;
volatile unsigned long last_interrupt_time = 0;
 
// -------------------------------------------------------------------------
 
void myInterrupt(void) {
   unsigned long interrupt_time = millis();
   if (interrupt_time - last_interrupt_time > 200) {
	int state = digitalRead(BUTTON_PIN);
	if (currentState != state){
		printf("Current state is: %d\n", state);
		currentState = state;
		if (state){
			int x = system("/opt/vc/bin/tvservice -p; chvt 6; chvt 7");
		}
		else{
			int x = system("/opt/vc/bin/tvservice -o");
		}
	}
   }
   last_interrupt_time = interrupt_time;
}
 
// -------------------------------------------------------------------------

static void skeleton_daemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    {
        close (x);
    }

    /* Open the log file */
    openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}

int main()
{
    skeleton_daemon();

  // sets up the wiringPi library
  if (wiringPiSetup () < 0) {
      fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
      return 1;
  }
 
  // set Pin 17/0 generate an interrupt on high-to-low transitions
  // and attach myInterrupt() to the interrupt
  if ( wiringPiISR (BUTTON_PIN, INT_EDGE_BOTH, &myInterrupt) < 0 ) {
      fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
      return 1;
  }
 
  // display counter value every second.
  while ( 1 ) {  }
 
    return EXIT_SUCCESS;
}
