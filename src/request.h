#ifndef REQUEST_H_
#define REQUEST_H_

#include "common.h"
#include "buffer.h"

class uv_handle_base_t;
class uv_tcp_socket_handle_t;
class uv_tcp_listen_handle_t;
class uv_udp_handle_t;
class uv_timer_handle_t;

#define REQUEST_SIZE_MAX				255
#define REQUEST_SPARE_MEMBER			m_spare
#define REQUEST_SPARE_SIZE(type)		(REQUEST_SIZE_MAX - offsetof(type, REQUEST_SPARE_MEMBER))
#define REQUEST_SIZE(type, extra)		(offsetof(type, REQUEST_SPARE_MEMBER) + (extra))
#define REQUEST_SPARE_PTR(request)		((request).REQUEST_SPARE_MEMBER)

enum request_type {
	REQUEST_EXIT,
	REQUEST_TCP_LISTEN,
	REQUEST_TCP_LISTENS,
	REQUEST_TCP_ACCEPT,
	REQUEST_TCP_CONNECT,
	REQUEST_TCP_CONNECTS,
	REQUEST_TCP_READ,
	REQUEST_TCP_WRITE,
	REQUEST_UDP_OPEN,
	REQUEST_UDP_READ,
	REQUEST_UDP_WRITE,
	REQUEST_HANDLE_OPTION,
	REQUEST_HANDLE_CLOSE,
	REQUEST_TIMER_START
};

#define REQUEST_SPARE_REGION			char REQUEST_SPARE_MEMBER[1];

struct request_exit_t {
	REQUEST_SPARE_REGION
};

struct request_tcp_listen_t {
	uint32_t m_source;
	uint32_t m_session;
	uint16_t m_port;
	bool m_ipv6;
	bool m_reuse;
	REQUEST_SPARE_REGION
};

struct request_tcp_listens_t {
	uint32_t m_source;
	uint32_t m_session;
	REQUEST_SPARE_REGION
};

struct request_tcp_accept_t {
	uv_tcp_listen_handle_t *m_listen_handle;
	bool m_blocking;
	REQUEST_SPARE_REGION
};

/* m_addrs is combination of remote host and local host, separated by '\0'. */
struct request_tcp_connect_t {
	uint32_t m_source;
	uint32_t m_session;
	uint16_t m_remote_port;
	uint16_t m_local_port;
	bool m_remote_ipv6;
	bool m_local_ipv6;
	REQUEST_SPARE_REGION
};

struct request_tcp_connects_t {
	uint32_t m_source;
	uint32_t m_session;
	REQUEST_SPARE_REGION
};

struct request_tcp_read_t {
	uv_tcp_socket_handle_t *m_socket_handle;
	bool m_blocking;
	REQUEST_SPARE_REGION
};

struct request_tcp_write_t {
	union {
		uv_tcp_socket_handle_t *m_socket_handle;
		uint64_t m_socket_fd;
	};
	union {
		const char* m_string;
		buffer_t m_buffer;
	};
	uint32_t m_length;   /* judge it's string or buffer */
	uint32_t m_source;   /* redundant if m_shared_write is true */
	uint32_t m_session;
	bool m_shared_write; /* whether m_socket_fd is valid */ 
	REQUEST_SPARE_REGION
};

struct request_udp_open_t {
	uint32_t m_source;
	uint32_t m_session;
	uint16_t m_port;
	bool m_ipv6;
	REQUEST_SPARE_REGION
};

struct request_udp_read_t {
	uv_udp_handle_t *m_socket_handle;
	REQUEST_SPARE_REGION
};

struct request_udp_write_t {
	union {
		uv_udp_handle_t *m_socket_handle;
		uint64_t m_socket_fd;
	};
	union {
		const char* m_string;
		buffer_t m_buffer;
	};
	uint32_t m_length;   /* judge it's string or buffer */
	uint32_t m_source;   /* redundant if m_shared_write is true */
	uint32_t m_session;
	uint16_t m_port;
	bool m_ipv6;
	bool m_shared_write; /* whether m_socket_fd is valid */
	REQUEST_SPARE_REGION
};

struct request_handle_option_t {
	uv_handle_base_t *m_handle;
	uint8_t m_option_type;
	REQUEST_SPARE_REGION
};

struct request_handle_close_t {
	uv_handle_base_t *m_handle_base;
	REQUEST_SPARE_REGION
};

struct request_timer_start_t {
	uv_timer_handle_t *m_timer_handle;
	uint64_t m_timeout; //in mili-seconds
	uint64_t m_repeat;  //in mili-seconds
	REQUEST_SPARE_REGION
};

struct request_t {
	uint8_t m_head_dummy[6];
	uint8_t m_length;
	uint8_t m_type;
	union {
		char m_body_dummy[REQUEST_SIZE_MAX];
		request_exit_t m_exit;
		request_tcp_listen_t m_tcp_listen;
		request_tcp_listens_t m_tcp_listens;
		request_tcp_accept_t m_tcp_accept;
		request_tcp_connect_t m_tcp_connect;
		request_tcp_connects_t m_tcp_connects;
		request_tcp_read_t m_tcp_read;
		request_tcp_write_t m_tcp_write;
		request_udp_open_t m_udp_open;
		request_udp_read_t m_udp_read;
		request_udp_write_t m_udp_write;
		request_handle_option_t m_handle_option;
		request_handle_close_t m_handle_close;
		request_timer_start_t m_timer_start;
	};
};

#endif
