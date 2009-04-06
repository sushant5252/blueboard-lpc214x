#ifndef _FS_API_H
#define _FS_API_H

#include "file.h"

#define MAX_NO_OF_MESSAGES  100
#define NUM_OF_BYTES_PER_CLUSTER 512

S32 fs_init(void);
S32 fs_close(void);
U8 fopen(File* file, S8 *filename,U8 open_mode);
S8 copy_fs_to_backup_area(void);
void ui_fs_test(void);
void set_file_system_status(U8 status);
U8 get_file_system_status(void);
U32 get_available_free_size(void);
U8 delete_file(U8 *name);



#endif
