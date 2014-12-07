/* mbtaskbuild.h -
 */

#ifndef _MBTASKBUILD_H_
#define _MBTASKBUILD_H_

#include "mbtask.h"
#include "mbtaskbuild-def.h"

G_BEGIN_DECLS



/* MbTaskBuild:
 */
struct _MbTaskBuild
{
  MB_TASK_BUILD_INSTANCE_HEADER;

  struct _MbRoom *room;
};



/* MbTaskBuildClass:
 */
struct _MbTaskBuildClass
{
  MB_TASK_BUILD_CLASS_HEADER;
};



MbTask *mb_task_build_new ( MbTask *parent,
                            struct _MbRoom *room );



G_END_DECLS

#endif /* ifndef _MBTASKBUILD_H_ */
