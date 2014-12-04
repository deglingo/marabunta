/* mbgame.h -
 */

#ifndef _MBGAME_H_
#define _MBGAME_H_

#include "mbbase.h"
#include "mbgame-def.h"

G_BEGIN_DECLS



/* MbGame:
 */
struct _MbGame
{
  MB_GAME_INSTANCE_HEADER;

  gpointer private;
};



/* MbGameClass:
 */
struct _MbGameClass
{
  MB_GAME_CLASS_HEADER;
};



MbGame *mb_game_new ( void );
void mb_game_setup ( MbGame *game );



G_END_DECLS

#endif /* ifndef _MBGAME_H_ */
