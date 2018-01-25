/* GET PROMISC FLAG FROM INTERFACE 
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Thomas Nogueira <t.nogueira94@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return 
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

int exit_on_failure(char* errormsg)
{
  printf("%s\n", errormsg);
  exit(1);
}

void check_arg(int c)
{
  if (c == 1 || c > 2)
    {
      printf("Usage ./getpromiscflag interfacename\n");
      exit(0);
    }   
}

int main (int c, char** argv)
{
  check_arg(c);
  struct ifreq ifr;
  int skfd =-1;
  int retioctl = -1;
  char *if_name;
  size_t if_name_len;
  if_name = argv[1];
  if_name_len=strlen(if_name);
  if (if_name_len<sizeof(ifr.ifr_name))
    {
      memcpy(ifr.ifr_name,if_name,if_name_len);
      ifr.ifr_name[if_name_len] = 0;
    }
  else
    exit_on_failure("Interface name to long");

  skfd=socket(AF_INET,SOCK_DGRAM,0);
  if (skfd == -1)
      exit_on_failure("Unable to open socket");

  retioctl = ioctl(skfd,SIOCGIFFLAGS,&ifr);
  if (retioctl == -1)
    exit_on_failure("ioctl call failed - please check your interface name ");

  if (ifr.ifr_flags & IFF_PROMISC)
    printf("%s Promisc set\n", ifr.ifr_name);
  else
    printf("%s Promisc unset\n", ifr.ifr_name);
  return(0);
}
