/*
 * Copyright (c) 2012 by Internet Systems Consortium, Inc. ("ISC")
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* Import. */

#include "private.h"

/* Private declarations. */

typedef enum {
	sockdir_invalid,
	sockdir_accept,
	sockdir_connect,
} sockdir_t;

typedef enum {
	socktype_invalid,
	socktype_pubsub,
	socktype_pushpull,
} socktype_t;

/* Forward. */

static bool
munge_endpoint(const char *, char **, sockdir_t *, socktype_t *);

static bool
set_options(void *, int);

/* Private. */

static bool
munge_endpoint(const char *ep, char **s_ep, sockdir_t *s_dir, socktype_t *s_type) {
	char *s, *saveptr, *tok;
	bool found_sockdir = false;
	bool found_socktype = false;

	s = strdup(ep);
	assert(s != NULL);

	*s_ep = strtok_r(s, ",", &saveptr);
	if (*s_ep == NULL) {
		free(s);
		return (false);
	}
	while ((tok = strtok_r(NULL, ",", &saveptr)) != NULL) {
		if (strcasecmp(tok, "accept") == 0) {
			if (found_sockdir) return (false);
			found_sockdir = true;
			*s_dir = sockdir_accept;
		} else if (strcasecmp(tok, "connect") == 0) {
			if (found_sockdir) return (false);
			found_sockdir = true;
			*s_dir = sockdir_connect;
		} else if (strcasecmp(tok, "pubsub") == 0) {
			if (found_socktype) return (false);
			found_socktype = true;
			*s_type = socktype_pubsub;
		} else if (strcasecmp(tok, "pushpull") == 0) {
			if (found_socktype) return (false);
			found_socktype = true;
			*s_type = socktype_pushpull;
		}
	}

	return (true);
}

static bool
set_options(void *s, int socket_type) {
	static const int i_thousand = 1000;

	if (socket_type == XS_SUB) {
		if (xs_setsockopt(s, XS_SUBSCRIBE, "NMSG", 4))
			return (false);
	} else if (socket_type == XS_PUB || socket_type == XS_PUSH) {
		if (xs_setsockopt(s, XS_SNDHWM, &i_thousand, sizeof(i_thousand)))
			return (false);
		if (xs_setsockopt(s, XS_LINGER, &i_thousand, sizeof(i_thousand)))
			return (false);
	}

	return (true);
}

/* Export. */

nmsg_input_t
nmsg_input_open_xs_endpoint(void *xs_ctx, const char *ep) {
	nmsg_input_t input = NULL;
	int socket_type;
	sockdir_t s_dir = sockdir_accept;
	socktype_t s_type = socktype_pubsub;
	char *s_ep = NULL;
	void *s;

	if (!munge_endpoint(ep, &s_ep, &s_dir, &s_type) || !s_ep)
		goto out;

	assert(s_dir == sockdir_accept || s_dir == sockdir_connect);
	assert(s_type == socktype_pubsub || s_type == socktype_pushpull);

	if (s_type == socktype_pubsub)
		socket_type = XS_SUB;
	else if (s_type == socktype_pushpull)
		socket_type = XS_PULL;

	s = xs_socket(xs_ctx, socket_type);
	if (!s) goto out;
	if (!set_options(s, socket_type)) {
		xs_close(s);
		goto out;
	}

	if (s_dir == sockdir_accept) {
		if (xs_bind(s, s_ep) == -1) {
			xs_close(s);
			goto out;
		}
	} else if (s_dir == sockdir_connect) {
		if (xs_connect(s, s_ep) == -1) {
			xs_close(s);
			goto out;
		}
	}

	input = nmsg_input_open_xs(s);
out:
	free(s_ep);
	return (input);
}

nmsg_output_t
nmsg_output_open_xs_endpoint(void *xs_ctx, const char *ep, size_t bufsz) {
	nmsg_output_t output = NULL;
	int socket_type;
	sockdir_t s_dir = sockdir_connect;
	socktype_t s_type = socktype_pubsub;
	char *s_ep = NULL;
	void *s;

	if (!munge_endpoint(ep, &s_ep, &s_dir, &s_type) || !s_ep)
		goto out;

	assert(s_dir == sockdir_accept || s_dir == sockdir_connect);
	assert(s_type == socktype_pubsub || s_type == socktype_pushpull);

	if (s_type == socktype_pubsub)
		socket_type = XS_PUB;
	else if (s_type == socktype_pushpull)
		socket_type = XS_PUSH;

	s = xs_socket(xs_ctx, socket_type);
	if (!s) goto out;
	if (!set_options(s, socket_type)) {
		xs_close(s);
		goto out;
	}

	if (s_dir == sockdir_accept) {
		if (xs_bind(s, s_ep) == -1) {
			xs_close(s);
			goto out;
		}
	} else if (s_dir == sockdir_connect) {
		if (xs_connect(s, s_ep) == -1) {
			xs_close(s);
			goto out;
		}
	}

	output = nmsg_output_open_xs(s, bufsz);
out:
	free(s_ep);
	return (output);
}
