/* mbalmain.c -
 */

#include <stdio.h>

#include "client/mbcclient.h"



/* main:
 */
int main ()
{
  GMainLoop *loop;
  MbcClient *cli;
  cli = mbc_client_new();
  printf("marabunta-al: hello!\n");
  mbc_client_connect(cli, "localhost", 6666);
  mbc_client_send(cli, "hola!");
  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
  return 0;
}
