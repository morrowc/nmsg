/*
 * Copyright (c) 2008, 2009 by Internet Systems Consortium, Inc. ("ISC")
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

#include "nmsg_port.h"

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "io_private.h"
#include "output.h"
#include "payload.h"

/* Forward. */

Nmsg__NmsgPayload *
_nmsg_io_make_nmsg_payload(struct nmsg_io_thr *, uint8_t *, size_t);

nmsg_res
_nmsg_io_write_nmsg(struct nmsg_io_thr *, struct nmsg_io_buf *, Nmsg__Nmsg *);

nmsg_res
_nmsg_io_write_nmsg_dup(struct nmsg_io_thr *, struct nmsg_io_buf *,
			const Nmsg__Nmsg *);

nmsg_res
_nmsg_io_write_nmsg_payload(struct nmsg_io_thr *, struct nmsg_io_buf *,
			    Nmsg__NmsgPayload *);

nmsg_res
_nmsg_io_write_pres(struct nmsg_io_thr *, struct nmsg_io_pres *,
		    const Nmsg__Nmsg *);

/* Export. */

Nmsg__NmsgPayload *
_nmsg_io_make_nmsg_payload(struct nmsg_io_thr *iothr, uint8_t *pbuf, size_t sz) {
	Nmsg__NmsgPayload *np;
	struct nmsg_io *io;
	struct nmsg_io_pres *iopres;

	io = iothr->io;
	iopres = iothr->iopres;

	np = malloc(sizeof(*np));
	if (np == NULL)
		return (NULL);
	np->base.descriptor = &nmsg__nmsg_payload__descriptor;
	np->base.n_unknown_fields = 0;
	np->base.unknown_fields = NULL;
	np->vid = iopres->pres->vid;
	np->msgtype = iopres->pres->msgtype;
	np->time_sec = iothr->now.tv_sec;
	np->time_nsec = iothr->now.tv_nsec;
	np->has_payload = 1;
	np->payload.len = sz;
	np->payload.data = pbuf;
	np->n_user = io->n_user;
	np->user = io->user;

	return (np);
}

nmsg_res
_nmsg_io_write_nmsg(struct nmsg_io_thr *iothr,
		    struct nmsg_io_buf *iobuf,
		    Nmsg__Nmsg *nmsg)
{
	Nmsg__NmsgPayload *np;
	nmsg_res res;
	unsigned n;

	pthread_mutex_lock(&iobuf->lock);
	for (n = 0; n < nmsg->n_payloads; n++) {
		np = nmsg->payloads[n];
		res = _nmsg_io_write_nmsg_payload(iothr, iobuf, np);
		nmsg->payloads[n] = NULL;
		if (res != nmsg_res_success) {
			pthread_mutex_unlock(&iobuf->lock);
			return (res);
		}
	}
	pthread_mutex_unlock(&iobuf->lock);
	return (nmsg_res_success);
}

nmsg_res
_nmsg_io_write_nmsg_dup(struct nmsg_io_thr *iothr,
			struct nmsg_io_buf *iobuf,
			const Nmsg__Nmsg *nmsg)
{
	Nmsg__NmsgPayload *np;
	nmsg_res res;
	unsigned n;

	pthread_mutex_lock(&iobuf->lock);
	for (n = 0; n < nmsg->n_payloads; n++) {
		np = nmsg_payload_dup(nmsg->payloads[n]);
		res = _nmsg_io_write_nmsg_payload(iothr, iobuf, np);
		if (res != nmsg_res_success) {
			pthread_mutex_unlock(&iobuf->lock);
			return (res);
		}
	}
	pthread_mutex_unlock(&iobuf->lock);
	return (nmsg_res_success);
}

nmsg_res
_nmsg_io_write_nmsg_payload(struct nmsg_io_thr *iothr,
			    struct nmsg_io_buf *iobuf,
			    Nmsg__NmsgPayload *np)
{
	nmsg_io io;
	nmsg_res res;
	struct nmsg_io_close_event ce;

	io = iothr->io;

	if (io->n_user > 0) {
		np->n_user = io->n_user;
		np->user = io->user;
	}
	res = nmsg_output_append(iobuf->buf, np);
	if (!(res == nmsg_res_success ||
	      res == nmsg_res_pbuf_written))
		return (nmsg_res_failure);
	if (res == nmsg_res_pbuf_written)
		iobuf->count_nmsg_out += 1;
	iobuf->count_nmsg_payload_out += 1;

	res = nmsg_res_success;

	if (io->count > 0 && iobuf->count_nmsg_payload_out % io->count == 0) {
		if (iobuf->user != NULL) {
			ce.buf = &iobuf->buf;
			ce.closetype = nmsg_io_close_type_count;
			ce.fdtype = nmsg_io_fd_type_output_nmsg;
			ce.io = io;
			ce.user = iobuf->user;
			nmsg_output_close(&iobuf->buf);
			io->closed_fp(&ce);
		} else {
			res = nmsg_res_stop;
		}
	}

	if (io->interval > 0 &&
	    ((unsigned) iothr->now.tv_sec - iobuf->last.tv_sec) >= io->interval)
	{
		if (iobuf->user != NULL) {
			memcpy(&iobuf->last, &iothr->now, sizeof(iothr->now));
			ce.buf = &iobuf->buf;
			ce.closetype = nmsg_io_close_type_interval;
			ce.fdtype = nmsg_io_fd_type_output_nmsg;
			ce.io = io;
			ce.user = iobuf->user;
			nmsg_output_close(&iobuf->buf);
			io->closed_fp(&ce);
		} else {
			res = nmsg_res_stop;
		}
	}
	return (res);
}

nmsg_res
_nmsg_io_write_pres(struct nmsg_io_thr *iothr,
		    struct nmsg_io_pres *iopres,
		    const Nmsg__Nmsg *nmsg)
{
	Nmsg__NmsgPayload *np;
	char *pres;
	char when[32];
	nmsg_pbmod mod;
	nmsg_res res;
	struct nmsg_io_close_event ce;
	struct nmsg_io *io;
	struct tm *tm;
	time_t t;
	unsigned n;

	io = iothr->io;
	res = nmsg_res_success;
	pthread_mutex_lock(&iopres->lock);
	for (n = 0; n < nmsg->n_payloads; n++) {
		np = nmsg->payloads[n];
		t = np->time_sec;
		tm = gmtime(&t);
		strftime(when, sizeof(when), "%Y-%m-%d %T", tm);
		mod = nmsg_pbmodset_lookup(io->ms, np->vid, np->msgtype);
		if (mod != NULL)
			res = nmsg_pbmod_pbuf_to_pres(mod, np, &pres,
						      io->endline);
		if (res != nmsg_res_success)
			return (res);
		if (io->quiet == false)
			fprintf(iopres->fp, "[%zu] %s.%09u [%d:%d %s %s] "
				"[%08x %08x] %s%s",
				np->has_payload ? np->payload.len : 0,
				when, np->time_nsec,
				np->vid, np->msgtype,
				nmsg_pbmodset_vid_to_vname(io->ms, np->vid),
				nmsg_pbmodset_msgtype_to_mname(io->ms, np->vid,
							       np->msgtype),
				np->n_user >= 1 ? np->user[0] : 0,
				np->n_user >= 2 ? np->user[1] : 0,
				io->endline, pres);
		else
			fputs(pres, iopres->fp);
		fputs(io->endline, iopres->fp);

		free(pres);
		iopres->count_pres_payload_out += 1;

		if (io->count > 0 && iopres->count_pres_payload_out % io->count == 0) {
			if (iopres->user != NULL) {
				ce.pres = &iopres->pres;
				ce.closetype = nmsg_io_close_type_count;
				ce.fdtype = nmsg_io_fd_type_output_pres;
				ce.io = io;
				ce.user = iopres->user;
				fclose(iopres->fp);
				io->closed_fp(&ce);
				iopres->fp = fdopen(iopres->pres->fd, "w");
				if (iopres->fp == NULL) {
					res = nmsg_res_failure;
					break;
				}
			} else {
				res = nmsg_res_stop;
				break;
			}
		}

		if (io->interval > 0 &&
		    ((unsigned) iothr->now.tv_sec - iopres->last.tv_sec)
		    >= io->interval)
		{
			if (iopres->user != NULL) {
				memcpy(&iopres->last, &iothr->now,
				       sizeof(iothr->now));
				ce.pres = &iopres->pres;
				ce.closetype = nmsg_io_close_type_interval;
				ce.fdtype = nmsg_io_fd_type_output_nmsg;
				ce.io = io;
				ce.user = iopres->user;
				fclose(iopres->fp);
				io->closed_fp(&ce);
				iopres->fp = fdopen(iopres->pres->fd, "w");
				if (iopres->fp == NULL) {
					res = nmsg_res_failure;
					break;
				}
			} else {
				res = nmsg_res_stop;
				break;
			}
		}
	}
	iopres->count_pres_out += 1;
	pthread_mutex_unlock(&iopres->lock);
	return (res);
}
