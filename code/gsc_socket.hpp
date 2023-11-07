#ifndef _GSC_SOCKET_HPP_
#define _GSC_SOCKET_HPP_

#include "gsc.hpp"

void gsc_socket_init();
void gsc_socket_bind();
void gsc_socket_connect();
void gsc_socket_send();
void gsc_socket_receive();
void gsc_socket_close();

#endif