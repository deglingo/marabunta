/* mbsbase.h -
 */

#ifndef _MBSBASE_H_
#define _MBSBASE_H_

#include <lptree.h>

#include <glib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>



/* [FIXME] these shuld be in common/mbbase.h */
#define STRERROR strerror(errno)

#define DIE(msg...) do {						\
	fprintf(stderr, "%s:%s:%d:ERROR: ",			\
			G_LOG_DOMAIN, __FILE__, __LINE__);	\
	fprintf(stderr, msg);						\
	fprintf(stderr, "\n");						\
	exit(1);									\
  } while (0)



#endif
