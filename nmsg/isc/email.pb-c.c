/* Generated by the protocol buffer compiler.  DO NOT EDIT! */

#include "email.pb-c.h"
void   nmsg__isc__email__init
                     (Nmsg__Isc__Email         *message)
{
  static Nmsg__Isc__Email init_value = NMSG__ISC__EMAIL__INIT;
  *message = init_value;
}
size_t nmsg__isc__email__get_packed_size
                     (const Nmsg__Isc__Email *message)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &nmsg__isc__email__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t nmsg__isc__email__pack
                     (const Nmsg__Isc__Email *message,
                      uint8_t       *out)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &nmsg__isc__email__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t nmsg__isc__email__pack_to_buffer
                     (const Nmsg__Isc__Email *message,
                      ProtobufCBuffer *buffer)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &nmsg__isc__email__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Nmsg__Isc__Email *
       nmsg__isc__email__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Nmsg__Isc__Email *)
     protobuf_c_message_unpack (&nmsg__isc__email__descriptor,
                                allocator, len, data);
}
void   nmsg__isc__email__free_unpacked
                     (Nmsg__Isc__Email *message,
                      ProtobufCAllocator *allocator)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &nmsg__isc__email__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor nmsg__isc__email__field_descriptors[8] =
{
  {
    "headers",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, has_headers),
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, headers),
    NULL,
    NULL,
    NULL,NULL    /* reserved1, reserved2 */
  },
  {
    "srcip",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, has_srcip),
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, srcip),
    NULL,
    NULL,
    NULL,NULL    /* reserved1, reserved2 */
  },
  {
    "srchost",
    4,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, has_srchost),
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, srchost),
    NULL,
    NULL,
    NULL,NULL    /* reserved1, reserved2 */
  },
  {
    "helo",
    5,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, has_helo),
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, helo),
    NULL,
    NULL,
    NULL,NULL    /* reserved1, reserved2 */
  },
  {
    "from",
    6,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, has_from),
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, from),
    NULL,
    NULL,
    NULL,NULL    /* reserved1, reserved2 */
  },
  {
    "rcpt",
    7,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_BYTES,
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, n_rcpt),
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, rcpt),
    NULL,
    NULL,
    NULL,NULL    /* reserved1, reserved2 */
  },
  {
    "type",
    8,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_ENUM,
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, has_type),
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, type),
    &nmsg__isc__email_type__descriptor,
    NULL,
    NULL,NULL    /* reserved1, reserved2 */
  },
  {
    "bodyurl",
    9,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_BYTES,
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, n_bodyurl),
    PROTOBUF_C_OFFSETOF(Nmsg__Isc__Email, bodyurl),
    NULL,
    NULL,
    NULL,NULL    /* reserved1, reserved2 */
  },
};
static const unsigned nmsg__isc__email__field_indices_by_name[] = {
  7,   /* field[7] = bodyurl */
  4,   /* field[4] = from */
  0,   /* field[0] = headers */
  3,   /* field[3] = helo */
  5,   /* field[5] = rcpt */
  2,   /* field[2] = srchost */
  1,   /* field[1] = srcip */
  6,   /* field[6] = type */
};
static const ProtobufCIntRange nmsg__isc__email__number_ranges[1 + 1] =
{
  { 2, 0 },
  { 0, 8 }
};
const ProtobufCMessageDescriptor nmsg__isc__email__descriptor =
{
  PROTOBUF_C_MESSAGE_DESCRIPTOR_MAGIC,
  "nmsg.isc.Email",
  "Email",
  "Nmsg__Isc__Email",
  "nmsg.isc",
  sizeof(Nmsg__Isc__Email),
  8,
  nmsg__isc__email__field_descriptors,
  nmsg__isc__email__field_indices_by_name,
  1,  nmsg__isc__email__number_ranges,
  NULL,NULL,NULL,NULL    /* reserved[1234] */
};
const ProtobufCEnumValue nmsg__isc__email_type__enum_values_by_number[5] =
{
  { "unknown", "NMSG__ISC__EMAIL_TYPE__UNKNOWN", 0 },
  { "spamtrap", "NMSG__ISC__EMAIL_TYPE__SPAMTRAP", 1 },
  { "rej_network", "NMSG__ISC__EMAIL_TYPE__REJ_NETWORK", 2 },
  { "rej_content", "NMSG__ISC__EMAIL_TYPE__REJ_CONTENT", 3 },
  { "rej_user", "NMSG__ISC__EMAIL_TYPE__REJ_USER", 4 },
};
static const ProtobufCIntRange nmsg__isc__email_type__value_ranges[] = {
{0, 0},{0, 5}
};
const ProtobufCEnumValueIndex nmsg__isc__email_type__enum_values_by_name[5] =
{
  { "rej_content", 3 },
  { "rej_network", 2 },
  { "rej_user", 4 },
  { "spamtrap", 1 },
  { "unknown", 0 },
};
const ProtobufCEnumDescriptor nmsg__isc__email_type__descriptor =
{
  PROTOBUF_C_ENUM_DESCRIPTOR_MAGIC,
  "nmsg.isc.EmailType",
  "EmailType",
  "Nmsg__Isc__EmailType",
  "nmsg.isc",
  5,
  nmsg__isc__email_type__enum_values_by_number,
  5,
  nmsg__isc__email_type__enum_values_by_name,
  1,
  nmsg__isc__email_type__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
