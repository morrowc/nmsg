/*
 * Copyright (c) 2018 by Farsight Security, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "nmsg.h"
#include "nmsg/asprintf.h"
#include "nmsg/alias.h"
#include "nmsg/chalias.h"
#include "nmsg/container.h"
#include "nmsg/msgmod.h"
#include "nmsg/vendors.h"
#include "nmsg/base/defs.h"
#include "nmsg/sie/defs.h"

#include "wdns.h"

#define NAME	"test-io"

#define UDP_PORT_BASE	10000
#define UDP_PORT_END	11000



static void
dummy_callback(nmsg_message_t msg, void *user)
{
	return;
}

static nmsg_message_t
make_message(void)
{
	nmsg_message_t m;
	nmsg_msgmod_t mm;
	size_t nf, i;
	uint32_t u32v;
	uint8_t *uptr;

	mm = nmsg_msgmod_lookup_byname("SIE", "dnsdedupe");
	assert(mm != NULL);

	m = nmsg_message_init(mm);

	assert(nmsg_message_get_num_fields(m, &nf) == nmsg_res_success);
	assert(nf != 0);

	/* 14 fields total */

	uptr = (uint8_t *)&u32v;
	u32v = 1;
	assert(nmsg_message_set_field(m, "count", 0, uptr, 4) == nmsg_res_success);
	u32v = 0x31337;
	assert(nmsg_message_set_field(m, "time_first", 0, uptr, 4) == nmsg_res_success);
	assert(nmsg_message_set_field(m, "zone_time_first", 0, uptr, 4) == nmsg_res_success);
	u32v += 0xbeef;
	assert(nmsg_message_set_field(m, "time_last", 0, uptr, 4) == nmsg_res_success);
	assert(nmsg_message_set_field(m, "zone_time_last", 0, uptr, 4) == nmsg_res_success);

	u32v = inet_addr("9.9.9.9");
	assert(nmsg_message_set_field(m, "response_ip", 0, uptr, 4) == nmsg_res_success);

	const char *rrname = "\x03""www""\x05""hello""\x0";

	assert(nmsg_message_set_field(m, "rrname", 0, (uint8_t *)rrname, strlen(rrname) + 1) == nmsg_res_success);

	u32v = WDNS_TYPE_A;
	assert(nmsg_message_set_field(m, "rrtype", 0, uptr, 4) == nmsg_res_success);

	u32v = WDNS_CLASS_IN;
	assert(nmsg_message_set_field(m, "rrclass", 0, uptr, 4) == nmsg_res_success);

	u32v = 3600;
	assert(nmsg_message_set_field(m, "rrttl", 0, uptr, 4) == nmsg_res_success);

//	u32v = 0;
//	assert(nmsg_message_set_field(m, "n_rdata", 0, uptr, 4) == nmsg_res_success);


/*
  size_t n_rdata;
  ProtobufCBinaryData *rdata;
  ProtobufCBinaryData response;
  ProtobufCBinaryData bailiwick; */


/*	for (i = 0; i < nf; i++) {
		assert(nmsg_message_set_field_by_idx(m, i, 0, (const uint8_t *)"ABCD", 4) == nmsg_res_success);
	}
*/

	return m;
}

static int
test_sock(void)
{
	nmsg_output_t o;
	nmsg_input_t i;
	nmsg_message_t mo, mi;
	struct timespec tnow, xtime;
	struct sockaddr_in s_in;
	int sfd, cfd;
	unsigned int nsrc = 0x1234;
	unsigned short lport = UDP_PORT_BASE;
	uint64_t count;

	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	assert(sfd != -1);

	memset(&s_in, 0, sizeof(s_in));
	s_in.sin_family = AF_INET;
	s_in.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_in.sin_port = htons(lport);
	assert(bind(sfd, (struct sockaddr *)&s_in, sizeof(s_in)) != -1);

	cfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	assert(cfd != -1);
	assert(connect(cfd, (struct sockaddr *)&s_in, sizeof(s_in)) != -1);

	i = nmsg_input_open_sock(sfd);
	assert(i != NULL);

	o = nmsg_output_open_sock(cfd, 8192);
	assert(o != NULL);

	nmsg_output_set_source(o, nsrc);

	/* First write is unbuffered. */
	mo = make_message();
	nmsg_output_set_buffered(o, false);
	assert(nmsg_output_write(o, mo) == nmsg_res_success);

	assert(nmsg_input_read(i, &mi) == nmsg_res_success);

	assert(nmsg_message_get_vid(mi) == NMSG_VENDOR_SIE_ID);
	assert(nmsg_message_get_msgtype(mi) == NMSG_VENDOR_SIE_DNSDEDUPE_ID);

	nmsg_message_destroy(&mo);

	/* Second write is buffered and must be flushed. */
	mo = make_message();
	nmsg_output_set_buffered(o, true);
	assert(nmsg_output_write(o, mo) == nmsg_res_success);
	assert(nmsg_output_flush(o) == nmsg_res_success);


	assert(nmsg_input_read(i, &mi) == nmsg_res_success);
	nmsg_message_destroy(&mo);

	/* Third write should be filtered by output, since it won't match. */
	nmsg_output_set_filter_msgtype_byname(o, "sie", "newdomain");
	mo = make_message();
	assert(nmsg_output_write(o, mo) == nmsg_res_success);
	assert(nmsg_output_flush(o) == nmsg_res_success);

	assert(nmsg_input_read(i, &mi) == nmsg_res_again);
	nmsg_message_destroy(&mo);


	/* Fourth write will have a filter that WILL match. */
	nmsg_output_set_filter_msgtype(o, NMSG_VENDOR_SIE_ID, NMSG_VENDOR_SIE_DNSDEDUPE_ID);
	mo = make_message();
	assert(nmsg_output_write(o, mo) == nmsg_res_success);
	assert(nmsg_output_flush(o) == nmsg_res_success);

	assert(nmsg_input_read(i, &mi) == nmsg_res_success);


	assert(nmsg_message_get_payload(mi) != NULL);
	assert(nmsg_message_get_payload_size(mi) == 66);

	nmsg_message_destroy(&mo);

	/* Test some other random things. */
	nmsg_output_set_group(o, 666);
	mo = make_message();
	assert(nmsg_output_write(o, mo) == nmsg_res_success);
	assert(nmsg_output_flush(o) == nmsg_res_success);

	assert(nmsg_input_read(i, &mi) == nmsg_res_success);
	nmsg_message_destroy(&mo);

	assert(nmsg_message_get_group(mi) == 666);


	nmsg_message_get_time(mi, &xtime);
	nmsg_timespec_get(&tnow);



	assert(nmsg_input_get_count_container_received(i, &count) == nmsg_res_success);
	assert(count == 4);
	assert(nmsg_input_get_count_container_dropped(i, &count) == nmsg_res_success);
	assert(count == 0);




	assert(nmsg_input_close(&i) == nmsg_res_success);
	assert(nmsg_output_close(&o) == nmsg_res_success);


	close(sfd);
	close(cfd);

	return 0;
}

static int
test_timing(void)
{
	nmsg_output_t o;
//	nmsg_input_t i;
	nmsg_message_t mo, mi;
	struct stat sb;
	FILE *f;
	int fd;
	size_t old_size;

	f = tmpfile();
	assert(f != NULL);

	fd = fileno(f);
	assert(fd != -1);

//	i = nmsg_input_open_sock(fd);
//	assert(i != NULL);

	o = nmsg_output_open_file(fd, 8192);
	assert(o != NULL);

	mo = make_message();
	const char *rrname = "\x03""www""\x50""AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA""\x03""com""\x0";
	assert(nmsg_message_set_field(mo, "rrname", 0, (uint8_t *)rrname, strlen(rrname) + 1) == nmsg_res_success);

	nmsg_output_set_buffered(o, false);
	assert(nmsg_output_write(o, mo) == nmsg_res_success);

//	assert(nmsg_input_read(i, &mi) == nmsg_res_success);


//	assert(nmsg_input_close(&i) == nmsg_res_success);

	assert(fstat(fd, &sb) != -1);
	old_size = sb.st_size;
	assert(lseek(fd, SEEK_SET, 0) == 0);
	assert(ftruncate(fd, 0) != -1);

	/* Rewrite the file with some compression turned on. New output should be smaller. */
	nmsg_output_set_zlibout(o, true);
	assert(nmsg_output_write(o, mo) == nmsg_res_success);
	assert(fstat(fd, &sb) != -1);
	assert(sb.st_size < (off_t)old_size);

	nmsg_message_destroy(&mo);
	assert(nmsg_output_close(&o) == nmsg_res_success);

	fclose(f);

	return 0;
}

static int
test_dummy(void)
{
	nmsg_input_t i, ij;
	nmsg_container_t c;
	nmsg_message_t m, *ms;
	nmsg_res res;
	uint8_t *buf;
	size_t n_ms = 0, nn_ms, bufsz;
	int fd;

	/* First create a container and load it up. */
	fd = open("./tests/generic-tests/dedupe.json", O_RDONLY);
	assert(fd != -1);

	ij = nmsg_input_open_json(fd);
	assert(ij != NULL);

	c = nmsg_container_init(8192);
	assert(c != NULL);

	while ((res = nmsg_input_read(ij, &m)) == nmsg_res_success) {
		n_ms++;
		assert(nmsg_container_add(c, m) == nmsg_res_success);
	}

	assert(res == nmsg_res_eof);

	/* Serialize the container and then read it through the null type */
	assert(nmsg_container_serialize(c, &buf, &bufsz, true, false, 0, 0) == nmsg_res_success);

	i = nmsg_input_open_null();
	assert(i != NULL);

	/*
	 * Directly from the nmsg comments:
	 *
	 * Calling nmsg_input_loop() or nmsg_input_read() on a "null source" input
	 * will fail. Callers instead need to use nmsg_input_read_null().
	 */
	assert(nmsg_input_loop(i, 1, dummy_callback, NULL) != nmsg_res_success);
	assert(nmsg_input_read(i, &m) != nmsg_res_success);

	assert(nmsg_input_read_null(i, buf, bufsz, NULL, &ms, &nn_ms) == nmsg_res_success);
	assert(n_ms == nn_ms);

	assert(nmsg_input_close(&ij) == nmsg_res_success);
	assert(nmsg_input_close(&i) == nmsg_res_success);
	nmsg_container_destroy(&c);

	return 0;
}

static int
test_multiplex(void)
{
	void *user = (void *)0xdeadbeef;
	size_t i = 0;
	size_t total;

	while (i < 2) {
		nmsg_io_t io;
		nmsg_input_t i1, i2;
		nmsg_input_t ij1, ij2, ij3;
		nmsg_output_t o1, o2;
		int fd, pipe_fds1[2], pipe_fds2[2], nread;
		size_t first_total = 0, second_total = 0;
		char tmpbuf1[8192], tmpbuf2[8192];

		io = nmsg_io_init();
		assert(io != NULL);

		i1 = nmsg_input_open_null();
		assert(i1 != NULL);

		assert(nmsg_io_add_input(io, i1, NULL) == nmsg_res_success);
		assert(nmsg_io_get_num_inputs(io) == 1);

		i2 = nmsg_input_open_null();
		assert(i2 != NULL);
		assert(nmsg_io_add_input(io, i2, NULL) == nmsg_res_success);
		assert(nmsg_io_get_num_inputs(io) == 2);

		fd = open("./tests/generic-tests/dedupe.json", O_RDONLY);
		assert(fd != -1);
		ij1 = nmsg_input_open_json(fd);
		assert(ij1 != NULL);
		assert(nmsg_io_add_input(io, ij1, NULL) == nmsg_res_success);

		fd = open("./tests/generic-tests/newdomain.json", O_RDONLY);
		assert(fd != -1);
		ij2 = nmsg_input_open_json(fd);
		assert(ij1 != NULL);
		assert(nmsg_io_add_input(io, ij2, NULL) == nmsg_res_success);

		fd = open("./tests/generic-tests/dedupe.json", O_RDONLY);
		assert(fd != -1);
		ij3 = nmsg_input_open_json(fd);
		assert(ij1 != NULL);
		assert(nmsg_io_add_input(io, ij3, NULL) == nmsg_res_success);

		assert(nmsg_io_get_num_inputs(io) == 5);

		assert(nmsg_io_get_num_outputs(io) == 0);

		assert(pipe(pipe_fds1) != -1);
		assert(pipe(pipe_fds2) != -1);

		o1 = nmsg_output_open_pres(pipe_fds1[1]);
		assert(o1 != NULL);
		o2 = nmsg_output_open_pres(pipe_fds2[1]);
		assert(o2 != NULL);

		assert(nmsg_io_add_output(io, o1, user) == nmsg_res_success);
		assert(nmsg_io_add_output(io, o2, user) == nmsg_res_success);
		assert(nmsg_io_get_num_outputs(io) == 2);

		if (!i)
			nmsg_io_set_output_mode(io, nmsg_io_output_mode_mirror);
		else
			nmsg_io_set_output_mode(io, nmsg_io_output_mode_stripe);

		nmsg_io_set_count(io, 15);
	//	nmsg_io_set_interval(io, 1);
	fprintf(stderr, "HEHE:        %d\n", nmsg_io_loop(io));
	//	assert(nmsg_io_loop(io) == nmsg_res_success);


		nmsg_io_destroy(&io);
		assert(io == NULL);
//		close(pipe_fds1[1]);
//		close(pipe_fds2[1]);

		memset(tmpbuf1, 0, sizeof(tmpbuf1));
		memset(tmpbuf2, 0, sizeof(tmpbuf2));

		while (first_total < sizeof(tmpbuf1)) {
			nread = read(pipe_fds1[0], tmpbuf1 + first_total, sizeof(tmpbuf1) - first_total);

			if (nread <= 0)
				break;

			first_total += nread;
		}

		while (second_total < sizeof(tmpbuf2)) {
			nread = read(pipe_fds2[0], tmpbuf2 + second_total, sizeof(tmpbuf2) - second_total);

			if (nread <= 0)
				break;

			second_total += nread;
		}

		if (!i)
			total = first_total + second_total;

//		fprintf(stderr, "HEHE: %zu vs %zu\n", first_total, second_total);

		if (!i) {
			assert(first_total == second_total);
			assert(!memcmp(tmpbuf1, tmpbuf2, first_total));
		} else {
			int diff;

			diff = (first_total != second_total ||
				(memcmp(tmpbuf1, tmpbuf2, first_total)));
			assert(diff != 0);
		}

		if (i) {
//fprintf(stderr, "HMMZ: t=%zu, %zu\n", total, first_total + second_total);
//			assert(total == (first_total + second_total));
		}

		i++;
	}

	return 0;
}

/*
 * A small amount of trickery and indirection is required here.
 * Certain filters are only applicable to nmsg inputs of type stream.
 * This precludes certain vehicles like data in json and presentation format.
 * While we could theoretically just open up a binary nmsg file with the
 * function nmsg_input_open_sock(), the subsequent read via recvfrom() would
 * fail on it since it's a local file and not a socket.
 *
 * Therefore we create a dummy fd with socketpair() and manually proxy
 * our locally stored nmsg data across it.
 */
static int
test_io_filters2(void)
{
	int n;

	for (n = 0; n < 11; n++) {
		nmsg_input_t i;
		nmsg_message_t m;
		int fd, sfds[2];

		assert(socketpair(AF_LOCAL, SOCK_STREAM, 0, sfds) != -1);

		fd = open("./tests/generic-tests/newdomain.nmsg", O_RDONLY);
		assert(fd != -1);

		i = nmsg_input_open_sock(sfds[0]);
		assert(i != NULL);

		/* Only need to try this once. */
		if (!n) {
			assert(nmsg_input_set_filter_msgtype_byname(i, "some_vendor", "nonexistent_type") != nmsg_res_success);
		}

		/* The ordering is particular. Every odd numbered test should
		 * succeed, and vice versa. */
		switch(n) {
			case 1:
				nmsg_input_set_filter_msgtype(i, NMSG_VENDOR_SIE_ID, NMSG_VENDOR_SIE_DNSDEDUPE_ID);
				break;
			case 2:
				nmsg_input_set_filter_msgtype(i, NMSG_VENDOR_SIE_ID, NMSG_VENDOR_SIE_NEWDOMAIN_ID);
				break;
			case 3:
				nmsg_input_set_filter_group(i, 2835122346);
				break;
			case 4:
				nmsg_input_set_filter_group(i, 0);
				break;
			case 5:
				nmsg_input_set_filter_source(i, 1235817825);
				break;
			case 6:
				nmsg_input_set_filter_source(i, 0xa1ba02cf);
				break;
			case 7:
				nmsg_input_set_filter_operator(i, 138158152);
				break;
			case 8:
				nmsg_input_set_filter_operator(i, 0);
				break;
			case 9:
				assert(nmsg_input_set_filter_msgtype_byname(i, "SIE", "dnsdedupe") == nmsg_res_success);
				break;
			case 10:
				assert(nmsg_input_set_filter_msgtype_byname(i, "SIE", "newdomain") == nmsg_res_success);
				break;
			default:
				break;
		}

		while (1) {
			char buf[1024];
			int nread;

			nread = read(fd, buf, sizeof(buf));

			if (nread <= 0)
				break;

			assert(write(sfds[1], buf, nread) == nread);
		}

		if (!(n % 2)) {
			assert(nmsg_input_read(i, &m) == nmsg_res_success);
		} else {
			assert(nmsg_input_read(i, &m) != nmsg_res_success);
		}

		assert(nmsg_input_close(&i) == nmsg_res_success);

		close(fd);
		close(sfds[1]);
	}

	return 0;
}

static int
test_rate(void)
{
	size_t all_rates[7] = { 30, 15, 10, 5, 4, 2, 1 };
	size_t n;
	double all_elapsed[7];

	memset(&all_elapsed, 0, sizeof(all_elapsed));

	for (n = 0; n < (sizeof(all_rates) / sizeof(all_rates[0])); n++) {
		struct timespec ts1, ts2;
		nmsg_rate_t r;
		nmsg_output_t o;
		nmsg_input_t i, ri;
		nmsg_message_t m;
		int fd, sfds[2];
		size_t n_success = 0;

		assert(socketpair(AF_LOCAL, SOCK_STREAM, 0, sfds) != -1);

		fd = open("./tests/generic-tests/dedupe.json", O_RDONLY);
		assert(fd != -1);

		i = nmsg_input_open_json(fd);
		assert(i != NULL);

		ri = nmsg_input_open_sock(sfds[0]);
		assert(ri != NULL);
		o = nmsg_output_open_sock(sfds[1], 8192);
		assert(o != NULL);
		nmsg_output_set_buffered(o, false);

		r = nmsg_rate_init(all_rates[n], 10);
		assert(r != NULL);

		nmsg_output_set_rate(o, r);
	//	assert(nmsg_input_set_byte_rate(ri, 1) == nmsg_res_success);

		nmsg_timespec_get(&ts1);

		while (nmsg_input_read(i, &m) == nmsg_res_success) {
			n_success++;

//			nmsg_rate_sleep(r);
			assert(nmsg_output_write(o, m) == nmsg_res_success);
			assert(nmsg_input_read(ri, &m) == nmsg_res_success);
		}

		nmsg_timespec_get(&ts2);

		assert(n_success == 5);

		nmsg_timespec_sub(&ts1, &ts2);
		all_elapsed[n] = nmsg_timespec_to_double(&ts2);

		if (n > 0) {
			assert(all_elapsed[n] > all_elapsed[n - 1]);
		}

		assert(all_elapsed[n] < ((double)5 / (double)all_rates[n]));

		assert(nmsg_input_close(&i) == nmsg_res_success);
		assert(nmsg_output_close(&o) == nmsg_res_success);
		nmsg_rate_destroy(&r);
	}

	return 0;
}


static void *user_data = (void *)0xdeadbeef;
static int touched_exit, touched_atstart, touched_close, num_received, touched_filter;

static void
test_close_fp(struct nmsg_io_close_event *ce)
{
	__sync_add_and_fetch(&touched_close, 1);

	return;
}

static void
test_atstart_fp(unsigned threadno, void *user)
{
	assert(user == user_data);
	__sync_add_and_fetch(&touched_atstart, 1);

	return;
}

static void
test_atexit_fp(unsigned threadno, void *user)
{
	assert(user == user_data);
	__sync_add_and_fetch(&touched_exit, 1);

	return;
}

static void
output_callback(nmsg_message_t msg, void *user)
{
	assert(user == user_data);
	__sync_add_and_fetch(&num_received, 1);

	return;
}

/* A filter to permit only msg type NMSG_VENDOR_SIE_DNSDEDUPE_ID */
static nmsg_res
filter_callback(nmsg_message_t *msg, void *user, nmsg_filter_message_verdict *vres)
{
	assert(user == user_data);

	if (nmsg_message_get_msgtype(*msg) == NMSG_VENDOR_SIE_DNSDEDUPE_ID)
		*vres = nmsg_filter_message_verdict_DROP;
	else
		*vres = nmsg_filter_message_verdict_ACCEPT;

	__sync_add_and_fetch(&touched_filter, 1);

	return nmsg_res_success;
}

/* Just to test the filter policy. */
static nmsg_res
filter_callback2(nmsg_message_t *msg, void *user, nmsg_filter_message_verdict *vres)
{
	assert(user == user_data);

	*vres = nmsg_filter_message_verdict_DECLINED;
	__sync_add_and_fetch(&touched_filter, 1);

	return nmsg_res_success;
}


static int
test_io_filters(void)
{
	nmsg_io_t io;
	nmsg_output_t o;
	size_t run_cnt = 0;

	/*
	 * Loop #1: Verify all 10 nmsgs read normally.
	 * Loop #2: Set count to 7 and verify 7 msgs read normally.
	 * Loop #3: Apply first filter callback. It should drop all msgs of type !=
	 *          dnsdedupe, meaning that half (5) of the packets will be dropped.
	 * Loop #4: Apply second filter callback.
	 */
	while (run_cnt < 5) {
		io = nmsg_io_init();
		assert(io != NULL);

		assert(nmsg_io_add_input_fname(io, "./tests/generic-tests/dedupe.nmsg", NULL) == nmsg_res_success);
		assert(nmsg_io_add_input_fname(io, "./tests/generic-tests/newdomain.nmsg", NULL) == nmsg_res_success);

		o = nmsg_output_open_callback(output_callback, user_data);
		assert(o != NULL);
		assert(nmsg_io_add_output(io, o, user_data) == nmsg_res_success);

		touched_atstart = touched_exit = touched_close = num_received = touched_filter = 0;
		nmsg_io_set_close_fp(io, test_close_fp);
		nmsg_io_set_atstart_fp(io, test_atstart_fp, user_data);
		nmsg_io_set_atexit_fp(io, test_atexit_fp, user_data);

		if (!run_cnt)
			nmsg_io_set_count(io, 10);
		else if (run_cnt == 1)
			nmsg_io_set_count(io, 7);
		else
			nmsg_io_set_count(io, 10);

		if (run_cnt == 2) {
			assert(nmsg_io_add_filter(io, filter_callback, user_data) == nmsg_res_success);
		} else if (run_cnt == 3) {
			assert(nmsg_io_add_filter(io, filter_callback2, user_data) == nmsg_res_success);
		} else if (run_cnt == 4) {
			assert(nmsg_io_add_filter(io, filter_callback2, user_data) == nmsg_res_success);
			/* XXX: This isn't working; it appears to be a bug in libnmsg */
			nmsg_io_set_filter_policy(io, nmsg_filter_message_verdict_DROP);

		}

		assert(nmsg_io_loop(io) == nmsg_res_success);

		nmsg_io_destroy(&io);
		assert(io == NULL);

//		fprintf(stderr, "SIZE: start = %d, close = %d, exit = %d;  num_inputs = %d\n", touched_atstart, touched_close, touched_exit, num_received);
//		fprintf(stderr, "touched filter: %d\n", touched_filter);

		assert(touched_atstart != 0);
		assert(touched_exit == touched_atstart);
		assert(touched_close >= touched_atstart);

		if (run_cnt == 2) {
			assert(touched_filter == 10);
			assert(num_received == 5);
		} else if (run_cnt == 3) {
			assert(touched_filter == 10);
			assert(num_received == 10);
		} else if (run_cnt == 4) {
			assert(touched_filter == 10);
			assert(num_received == 10);
		} else {
			assert(touched_filter == 0);
			assert(num_received == 10);
		}

		run_cnt++;
	}

	return 0;
}

static int
check(int ret, const char *s)
{
	if (ret == 0) {
		fprintf(stderr, NAME ": PASS: %s\n", s);
	} else {
		fprintf(stderr, NAME ": FAIL: %s\n", s);
	}
	return ret;
}

int
main(void)
{
	int ret = 0;

	assert(nmsg_init() == nmsg_res_success);

	ret |= check(test_dummy(), "test-io");
	ret |= check(test_multiplex(), "test-io");
	ret |= check(test_sock(), "test-io");
	ret |= check(test_timing(), "test-io");
	ret |= check(test_io_filters(), "test-io");
	ret |= check(test_io_filters2(), "test-io");
	ret |= check(test_rate(), "test-io");

	if (ret)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}