/*
 * Copyright (c) 2009-2012 by Farsight Security, Inc.
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

#include "private.h"

#include "transparent.h"

#ifdef HAVE_YAJL
nmsg_res
_nmsg_msgmod_json_to_payload(struct nmsg_msgmod *mod, void *cl, const char *json) {
	return (nmsg_res_notimpl);
}

nmsg_res
_nmsg_msgmod_json_to_payload_load(struct nmsg_msgmod_field *field,
				  struct nmsg_msgmod_clos *clos,
				  const char *value, void *ptr, int *qptr)
{
	return (nmsg_res_notimpl);
}

nmsg_res
_nmsg_msgmod_json_to_payload_finalize(struct nmsg_msgmod *mod, void *cl,
				      uint8_t **pbuf, size_t *sz)
{
	return (nmsg_res_notimpl);
}
#else /* HAVE_YAJL */
nmsg_res
_nmsg_msgmod_json_to_payload(struct nmsg_msgmod *mod, void *cl, const char *json) {
	return (nmsg_res_notimpl);
}

nmsg_res
_nmsg_msgmod_json_to_payload_load(struct nmsg_msgmod_field *field,
				  struct nmsg_msgmod_clos *clos,
				  const char *value, void *ptr, int *qptr)
{
	return (nmsg_res_notimpl);
}

nmsg_res
_nmsg_msgmod_json_to_payload_finalize(struct nmsg_msgmod *mod, void *cl,
				      uint8_t **pbuf, size_t *sz)
{
	return (nmsg_res_notimpl);
}
#endif /* HAVE_YAJL */
