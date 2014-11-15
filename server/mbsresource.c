/* mbsresource.c -
 */

#include "server/mbsresource.h"
#include "server/mbsresource.inl"



/* mbs_resource_new:
 */
MbsResource *mbs_resource_new ( const gchar *name,
                                MbResourceFlags flags )
{
  MbsResource *rsc;
  rsc = MBS_RESOURCE_NEW(NULL);
  rsc->name = g_strdup(name);
  rsc->flags = flags;
  return rsc;
}
