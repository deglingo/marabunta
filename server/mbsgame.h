/* mbsgame.h -
 */

#ifndef _MBSGAME_H_
#define _MBSGAME_H_

#include "server/mbsbase.h"
#include "server/mbsworld.h"
#include "server/mbsgame-def.h"



typedef gpointer MbsPlayerID;

typedef void (* MbsMessageHandler) ( MbsPlayerID player,
                                     MbMessage *message,
                                     gpointer data );



/* MbsGame:
 */
struct _MbsGame
{
  MBS_GAME_INSTANCE_HEADER;

  GList *players;
  /* game timer */
  gdouble fps;
  GTimer *timer;
  guint frame;
  gdouble next_frame;
  /* game data */
  MbsWorld *world;
};



/* MbsGameClass:
 */
struct _MbsGameClass
{
  MBS_GAME_CLASS_HEADER;
};



MbsGame *mbs_game_new ( void );                        
MbsPlayerID mbs_game_add_player ( MbsGame *game,
                                  const gchar *name,
                                  MbsMessageHandler message_handler,
                                  gpointer message_data,
                                  GDestroyNotify destroy_data );
void mbs_game_start ( MbsGame *game );



#endif
