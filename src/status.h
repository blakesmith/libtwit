#ifndef LIBTWIT_STATUS_H
#define LIBTWIT_STATUS_H

#include "core.h"

int check_update_length(char *message);

struct tweet *parse_friends_timeline();

struct tweet *parse_user_timeline();

int send_update(char *message);
#endif
