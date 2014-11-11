#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "gpio-utils.h"

int keepgoing = 1;

void signal_handler(int sig);
// Callback called when SIGINT is sent to the process (Ctrl-C)
void signal_handler(int sig)
{
	printf( "Ctrl-C pressed, cleaning up and exiting..\n" );
	keepgoing = 0;
}

int main(int argc, char **argv, char **envp){
	int gpio_num;
	int gpio_value;


	printf("enter a gpio pin: ");
	scanf("%d", &gpio_num);
	printf("The pin is %d\n", gpio_num);
	gpio_export(gpio_num);
	gpio_set_dir(gpio_num, "in");
	gpio_get_value(gpio_num, &gpio_value);
	printf("The value is %d \n", gpio_value);


}
