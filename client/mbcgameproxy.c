/* mbcgameproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcgameproxy.h"
#include "client/mbcgameproxy.inl"



/* properties:
 */
enum
  {
    PROP_0,
    PROP_FRAME,
    PROP_COUNT,
  };

static LParamSpec *pspecs[PROP_COUNT] = { NULL, };



/* Signals:
 */
enum
  {
    SIG_STARTED,
    SIG_COUNT,
  };

static LSignalID signals[SIG_COUNT] = { 0, };



/* mbc_game_proxy_class_init:
 */
static void mbc_game_proxy_class_init ( LObjectClass *cls )
{
  pspecs[PROP_FRAME] =
    l_param_spec_int("frame", 0);

  l_object_class_install_properties(cls, PROP_COUNT, pspecs);

  signals[SIG_STARTED] =
    l_signal_new(cls,
                 "started");
}



/* mbc_game_proxy_new:
 */
MbcGameProxy *mbc_game_proxy_new ( void )
{
  MbcGameProxy *gp;
  gp = MBC_GAME_PROXY(l_object_new(MBC_CLASS_GAME_PROXY, NULL));
  gp->world = mbc_world_proxy_new();
  return gp;
}



/* mbc_game_proxy:
 */
void mbc_game_proxy_process_update ( MbcGameProxy *proxy,
                                     MbMessage *message )
{
  CL_DEBUG("[TODO] %d", message->frame);
  proxy->frame = message->frame;
  l_object_notify(L_OBJECT(proxy), pspecs[PROP_FRAME]);
}



/* mbc_game_proxy_reset:
 */
void mbc_game_proxy_reset ( MbcGameProxy *proxy )
{
  CL_DEBUG("[TODO] reset");
  proxy->frame = 0;
}



/* mbc_game_proxy_started:
 */
void mbc_game_proxy_started ( MbcGameProxy *proxy )
{
  l_signal_emit(L_OBJECT(proxy), signals[SIG_STARTED], 0);
}
