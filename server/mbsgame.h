/* mbsgame.h -
 */

#ifndef _MBSGAME_H_
#define _MBSGAME_H_

#include "server/mbsobject.h"
#include "server/mbsworld.h"
#include "server/mbsgame-def.h"



typedef gint MbsPlayerID;

typedef void (* MbsMessageHandler) ( MbsPlayerID player,
                                     MbMessage *message,
                                     gpointer data );



/* MbsGame:
 */
struct _MbsGame
{
  MBS_GAME_INSTANCE_HEADER;

  gpointer private;
  
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
void mbs_game_register_control ( MbsGame *game,
                                 MbsObject *control );
MbsObject *mbs_game_lookup_control ( MbsGame *game,
                                     MbsObjectID id );
void mbs_game_start ( MbsGame *game );
void mbs_game_handle_message ( MbsGame *game,
                               MbMessage *msg );



#endif
