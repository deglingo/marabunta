/* mbalapp.c -
 */

#include "client-al/mbalapp.h"
#include "client-al/mbalapp.inl"



/* mbal_app_new:
 */
MbalApp *mbal_app_new ( void )
{
  return MBAL_APP(l_object_new(MBAL_CLASS_APP, NULL));
}
