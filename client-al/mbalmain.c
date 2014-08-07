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
  return 0;
}
