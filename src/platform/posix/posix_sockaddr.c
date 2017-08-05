//
// Copyright 2017 Garrett D'Amore <garrett@damore.org>
// Copyright 2017 Capitar IT Group BV <info@capitar.com>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.
//

#include "core/nng_impl.h"

#ifdef PLATFORM_POSIX_SOCKADDR

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int
nni_posix_nn2sockaddr(void *sa, const nni_sockaddr *na)
{
	struct sockaddr_in *    sin;
	struct sockaddr_in6 *   sin6;
	const nng_sockaddr_in * nsin;
	const nng_sockaddr_in6 *nsin6;

	switch (na->s_un.s_family) {
	case NNG_AF_INET:
		sin  = (void *) sa;
		nsin = &na->s_un.s_in;
		memset(sin, 0, sizeof(*sin));
		sin->sin_family      = PF_INET;
		sin->sin_port        = nsin->sa_port;
		sin->sin_addr.s_addr = nsin->sa_addr;
		return (sizeof(*sin));

	case NNG_AF_INET6:
		sin6  = (void *) sa;
		nsin6 = &na->s_un.s_in6;
		memset(sin6, 0, sizeof(*sin6));
#ifdef SIN6_LEN
		sin6->sin6_len = sizeof(*sin6);
#endif
		sin6->sin6_family = PF_INET6;
		sin6->sin6_port   = nsin6->sa_port;
		memcpy(sin6->sin6_addr.s6_addr, nsin6->sa_addr, 16);
		return (sizeof(*sin6));
	}
	return (-1);
}

int
nni_posix_sockaddr2nn(nni_sockaddr *na, const void *sa)
{
	const struct sockaddr_in * sin;
	const struct sockaddr_in6 *sin6;
	nng_sockaddr_in *          nsin;
	nng_sockaddr_in6 *         nsin6;

	switch (((struct sockaddr *) sa)->sa_family) {
	case AF_INET:
		sin             = (void *) sa;
		nsin            = &na->s_un.s_in;
		nsin->sa_family = NNG_AF_INET;
		nsin->sa_port   = sin->sin_port;
		nsin->sa_addr   = sin->sin_addr.s_addr;
		break;
	case AF_INET6:
		sin6             = (void *) sa;
		nsin6            = &na->s_un.s_in6;
		nsin6->sa_family = NNG_AF_INET6;
		nsin6->sa_port   = sin6->sin6_port;
		memcpy(nsin6->sa_addr, sin6->sin6_addr.s6_addr, 16);
		break;
	default:
		// We should never see this - the OS should always be
		// specific about giving us either AF_INET or AF_INET6.
		// Other address families are not handled here.
		return (-1);
	}
	return (0);
}

#else

// Suppress empty symbols warnings in ranlib.
int nni_posix_sockaddr_not_used = 0;

#endif // PLATFORM_POSIX_SOCKADDR