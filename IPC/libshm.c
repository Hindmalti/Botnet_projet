#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <libthrd.h>
#include <stdio.h>
#include <stdlib.h>

struct ipc_perm
{
    uid_t uid;   /* numéro du propriétaire de l'objet */
    gid_t gid;   /* numéro de groupe de l'objet  */
    uid_t cuid;  /* numéro du créateur de l'objet */
    gid_t cgid;  /* numéro de groupe de l'objet */
    mode_t mode; /* droits de l'objet  */
    u_long seq;  /* séquence numérique : numéro quelconque */
    key_t key;   /* clef de ref de l'objet  */
};
int shmget (key_t clef, [paramètres ou pas], int option);