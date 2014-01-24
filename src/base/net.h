/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __NETWORK_H
#define __NETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

void 		slave(void);
int 		tcp_client(void);
//int 		net_connect(void);

#ifdef __cplusplus
}
#endif

#endif

