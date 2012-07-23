/*
  Changes:
  Sat Mar 20 09:51:38 EST 1999 Arnaldo Carvalho de Melo <acme@conectiva.com.br>
	Internationalization
*/
#include <stdio.h>              /* stderr */
#include <string.h>             /* strstr */
#include <unistd.h>             /* write */

#include <endian.h>

#include "common.h"
#include "fdisk.h"
#include "fdiskaixlabel.h"
#include "nls.h"

#define aixlabel ((aix_partition *)cxt->firstsector)

static	int     other_endian = 0;
static  short	volumes=1;

/*
 * only dealing with free blocks here
 */

static void
aix_info( void ) {
    puts(
	_("\n\tThere is a valid AIX label on this disk.\n"
	"\tUnfortunately Linux cannot handle these\n"
	"\tdisks at the moment.  Nevertheless some\n"
	"\tadvice:\n"
	"\t1. fdisk will destroy its contents on write.\n"
	"\t2. Be sure that this disk is NOT a still vital\n"
	"\t   part of a volume group. (Otherwise you may\n"
	"\t   erase the other disks as well, if unmirrored.)\n"
	"\t3. Before deleting this physical volume be sure\n"
	"\t   to remove the disk logically from your AIX\n"
	"\t   machine.  (Otherwise you become an AIXpert).")
    );
}

void
aix_nolabel(struct fdisk_context *cxt)
{
    aixlabel->magic = 0;
    partitions = 4;
    fdisk_zeroize_firstsector(cxt);
    return;
}

static int aix_probe_label(struct fdisk_context *cxt)
{
    if (aixlabel->magic != AIX_LABEL_MAGIC &&
	aixlabel->magic != AIX_LABEL_MAGIC_SWAPPED) {
	other_endian = 0;
	return 0;
    }
    other_endian = (aixlabel->magic == AIX_LABEL_MAGIC_SWAPPED);
    disklabel = AIX_LABEL;
    partitions= 1016;
    volumes = 15;
    aix_info();
    aix_nolabel(cxt);		/* %% */
    return 1;
}

const struct fdisk_label aix_label =
{
	.name = "aix",
	.probe = aix_probe_label,
	.write = NULL,
	.part_delete = NULL,
};
