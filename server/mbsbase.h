/* mbsbase.h -
 */

#ifndef _MBSBASE_H_
#define _MBSBASE_H_

#include <glib.h>

#include <stdio.h>
#include <stdlib.h>



/* [FIXME] these shuld be in common/mbbase.h */
#define DIE(msg...) do {						\
	fprintf(stderr, "%s:%s:%d:ERROR: ",			\
			G_LOG_DOMAIN, __FILE__, __LINE__);	\
	fprintf(stderr, msg);						\
	fprintf(stderr, "\n");						\
	exit(1);									\
  } while (0)



#endif
