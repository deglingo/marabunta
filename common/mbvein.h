/* mbvein.h -
 */

#ifndef _MBVEIN_H_
#define _MBVEIN_H_

#include "common/mbobject.h"
#include "common/mbvein-def.h"

G_BEGIN_DECLS



/* MbVein:
 */
struct _MbVein
{
  MB_VEIN_INSTANCE_HEADER;

  MbObject *sector;
  MbObject *resource;
  gint depth;
  gint64 qtty;
};



/* MbVeinClass:
 */
struct _MbVeinClass
{
  MB_VEIN_CLASS_HEADER;
};



#define MB_VEIN_SECTOR(v) (MB_VEIN(v)->sector)
#define MB_VEIN_RESOURCE(v) (MB_VEIN(v)->resource)
#define MB_VEIN_DEPTH(v) (MB_VEIN(v)->depth)
#define MB_VEIN_QTTY(v) (MB_VEIN(v)->qtty)



G_END_DECLS

#endif /* ifndef _MBVEIN_H_ */
