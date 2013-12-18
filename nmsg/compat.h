#ifndef NMSG_COMPAT_H
#define NMSG_COMPAT_H

#define NMSG_VENDOR_ISC_NCAP_ID				NMSG_VENDOR_BASE_NCAP_ID
#define NMSG_VENDOR_ISC_NCAP_NAME			NMSG_VENDOR_BASE_NCAP_NAME
#define NMSG_VENDOR_ISC_EMAIL_ID			NMSG_VENDOR_BASE_EMAIL_ID
#define NMSG_VENDOR_ISC_EMAIL_NAME			NMSG_VENDOR_BASE_EMAIL_NAME
#define NMSG_VENDOR_ISC_LINKPAIR_ID			NMSG_VENDOR_BASE_LINKPAIR_ID
#define NMSG_VENDOR_ISC_LINKPAIR_NAME			NMSG_VENDOR_BASE_LINKPAIR_NAME
#define NMSG_VENDOR_ISC_HTTP_ID				NMSG_VENDOR_BASE_HTTP_ID
#define NMSG_VENDOR_ISC_HTTP_NAME			NMSG_VENDOR_BASE_HTTP_NAME
#define NMSG_VENDOR_ISC_IPCONN_ID			NMSG_VENDOR_BASE_IPCONN_ID
#define NMSG_VENDOR_ISC_IPCONN_NAME			NMSG_VENDOR_BASE_IPCONN_NAME
#define NMSG_VENDOR_ISC_LOGLINE_ID			NMSG_VENDOR_BASE_LOGLINE_ID
#define NMSG_VENDOR_ISC_LOGLINE_NAME			NMSG_VENDOR_BASE_LOGLINE_NAME
#define NMSG_VENDOR_ISC_DNS_ID				NMSG_VENDOR_BASE_DNS_ID
#define NMSG_VENDOR_ISC_DNS_NAME			NMSG_VENDOR_BASE_DNS_NAME
#define NMSG_VENDOR_ISC_PKT_ID				NMSG_VENDOR_BASE_PKT_ID
#define NMSG_VENDOR_ISC_PKT_NAME			NMSG_VENDOR_BASE_PKT_NAME
#define NMSG_VENDOR_ISC_DNSQR_ID			NMSG_VENDOR_BASE_DNSQR_ID
#define NMSG_VENDOR_ISC_DNSQR_NAME			NMSG_VENDOR_BASE_DNSQR_NAME
#define NMSG_VENDOR_ISC_XML_ID				NMSG_VENDOR_BASE_XML_ID
#define NMSG_VENDOR_ISC_XML_NAME			NMSG_VENDOR_BASE_XML_NAME
#define NMSG_VENDOR_ISC_ENCODE_ID			NMSG_VENDOR_BASE_ENCODE_ID
#define NMSG_VENDOR_ISC_ENCODE_NAME			NMSG_VENDOR_BASE_ENCODE_NAME

#define Nmsg__Isc__Dns					Nmsg__Base__Dns
#define Nmsg__Isc__DnsQR				Nmsg__Base__DnsQR
#define Nmsg__Isc__DnsQRType				Nmsg__Base__DnsQRType
#define Nmsg__Isc__Email				Nmsg__Base__Email
#define Nmsg__Isc__EmailType				Nmsg__Base__EmailType
#define Nmsg__Isc__Encode				Nmsg__Base__Encode
#define Nmsg__Isc__EncodeType				Nmsg__Base__EncodeType
#define Nmsg__Isc__Http					Nmsg__Base__Http
#define Nmsg__Isc__HttpType				Nmsg__Base__HttpType
#define Nmsg__Isc__IPConn				Nmsg__Base__IPConn
#define Nmsg__Isc__Linkpair				Nmsg__Base__Linkpair
#define Nmsg__Isc__Linktype				Nmsg__Base__Linktype
#define Nmsg__Isc__LogLine				Nmsg__Base__LogLine
#define Nmsg__Isc__Ncap					Nmsg__Base__Ncap
#define Nmsg__Isc__NcapLegacyType			Nmsg__Base__NcapLegacyType
#define Nmsg__Isc__NcapType				Nmsg__Base__NcapType
#define Nmsg__Isc__Pkt					Nmsg__Base__Pkt
#define Nmsg__Isc__UdpChecksum				Nmsg__Base__UdpChecksum
#define Nmsg__Isc__Xml					Nmsg__Base__Xml

#define NMSG__ISC__DNS_QRTYPE__ICMP			NMSG__BASE__DNS_QRTYPE__ICMP
#define NMSG__ISC__DNS_QRTYPE__TCP			NMSG__BASE__DNS_QRTYPE__TCP
#define NMSG__ISC__DNS_QRTYPE__UDP_INVALID		NMSG__BASE__DNS_QRTYPE__UDP_INVALID
#define NMSG__ISC__DNS_QRTYPE__UDP_QUERY_ONLY		NMSG__BASE__DNS_QRTYPE__UDP_QUERY_ONLY
#define NMSG__ISC__DNS_QRTYPE__UDP_QUERY_RESPONSE	NMSG__BASE__DNS_QRTYPE__UDP_QUERY_RESPONSE
#define NMSG__ISC__DNS_QRTYPE__UDP_RESPONSE_ONLY	NMSG__BASE__DNS_QRTYPE__UDP_RESPONSE_ONLY
#define NMSG__ISC__DNS_QRTYPE__UDP_UNANSWERED_QUERY	NMSG__BASE__DNS_QRTYPE__UDP_UNANSWERED_QUERY
#define NMSG__ISC__DNS_QRTYPE__UDP_UNSOLICITED_RESPONSE	NMSG__BASE__DNS_QRTYPE__UDP_UNSOLICITED_RESPONSE
#define NMSG__ISC__EMAIL_TYPE__REJ_CONTENT		NMSG__BASE__EMAIL_TYPE__REJ_CONTENT
#define NMSG__ISC__EMAIL_TYPE__REJ_NETWORK		NMSG__BASE__EMAIL_TYPE__REJ_NETWORK
#define NMSG__ISC__EMAIL_TYPE__REJ_USER			NMSG__BASE__EMAIL_TYPE__REJ_USER
#define NMSG__ISC__EMAIL_TYPE__SPAMTRAP			NMSG__BASE__EMAIL_TYPE__SPAMTRAP
#define NMSG__ISC__EMAIL_TYPE__UNKNOWN			NMSG__BASE__EMAIL_TYPE__UNKNOWN
#define NMSG__ISC__EMAIL_TYPE__rej_content		NMSG__BASE__EMAIL_TYPE__rej_content
#define NMSG__ISC__EMAIL_TYPE__rej_network		NMSG__BASE__EMAIL_TYPE__rej_network
#define NMSG__ISC__EMAIL_TYPE__rej_user			NMSG__BASE__EMAIL_TYPE__rej_user
#define NMSG__ISC__EMAIL_TYPE__spamtrap			NMSG__BASE__EMAIL_TYPE__spamtrap
#define NMSG__ISC__EMAIL_TYPE__unknown			NMSG__BASE__EMAIL_TYPE__unknown
#define NMSG__ISC__ENCODE_TYPE__JSON			NMSG__BASE__ENCODE_TYPE__JSON
#define NMSG__ISC__ENCODE_TYPE__MSGPACK			NMSG__BASE__ENCODE_TYPE__MSGPACK
#define NMSG__ISC__ENCODE_TYPE__TEXT			NMSG__BASE__ENCODE_TYPE__TEXT
#define NMSG__ISC__ENCODE_TYPE__XML			NMSG__BASE__ENCODE_TYPE__XML
#define NMSG__ISC__ENCODE_TYPE__YAML			NMSG__BASE__ENCODE_TYPE__YAML
#define NMSG__ISC__HTTP_TYPE__SINKHOLE			NMSG__BASE__HTTP_TYPE__SINKHOLE
#define NMSG__ISC__HTTP_TYPE__UNKNOWN			NMSG__BASE__HTTP_TYPE__UNKNOWN
#define NMSG__ISC__HTTP_TYPE__sinkhole			NMSG__BASE__HTTP_TYPE__sinkhole
#define NMSG__ISC__HTTP_TYPE__unknown			NMSG__BASE__HTTP_TYPE__unknown
#define NMSG__ISC__LINKTYPE__ANCHOR			NMSG__BASE__LINKTYPE__ANCHOR
#define NMSG__ISC__LINKTYPE__REDIRECT			NMSG__BASE__LINKTYPE__REDIRECT
#define NMSG__ISC__LINKTYPE__anchor			NMSG__BASE__LINKTYPE__anchor
#define NMSG__ISC__LINKTYPE__redirect			NMSG__BASE__LINKTYPE__redirect
#define NMSG__ISC__NCAP_LEGACY_TYPE__ICMP		NMSG__BASE__NCAP_LEGACY_TYPE__ICMP
#define NMSG__ISC__NCAP_LEGACY_TYPE__TCP		NMSG__BASE__NCAP_LEGACY_TYPE__TCP
#define NMSG__ISC__NCAP_LEGACY_TYPE__UDP		NMSG__BASE__NCAP_LEGACY_TYPE__UDP
#define NMSG__ISC__NCAP_TYPE__IPV4			NMSG__BASE__NCAP_TYPE__IPV4
#define NMSG__ISC__NCAP_TYPE__IPV6			NMSG__BASE__NCAP_TYPE__IPV6
#define NMSG__ISC__NCAP_TYPE__LEGACY			NMSG__BASE__NCAP_TYPE__LEGACY
#define NMSG__ISC__NCAP_TYPE__Legacy			NMSG__BASE__NCAP_TYPE__Legacy
#define NMSG__ISC__UDP_CHECKSUM__ABSENT			NMSG__BASE__UDP_CHECKSUM__ABSENT
#define NMSG__ISC__UDP_CHECKSUM__CORRECT		NMSG__BASE__UDP_CHECKSUM__CORRECT
#define NMSG__ISC__UDP_CHECKSUM__ERROR			NMSG__BASE__UDP_CHECKSUM__ERROR
#define NMSG__ISC__UDP_CHECKSUM__INCORRECT		NMSG__BASE__UDP_CHECKSUM__INCORRECT

#endif /* NMSG_COMPAT_H */
