/* mbalmain.c -
 */

#include <stdio.h>

#include "client/mbcclient.h"



/* main:
 */
int main ()
{
  MbcClient *cli;
  cli = mbc_client_new();
  printf("marabunta-al: hello!\n");
  mbc_client_connect(cli, "localhost", 6666);
  mbc_client_send(cli, "hola!");
  return 0;
}
