/*
 * Copyright (C) 2003-2013 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MPD_OUTPUT_HTTPD_CLIENT_HXX
#define MPD_OUTPUT_HTTPD_CLIENT_HXX

#include "gcc.h"

#include <glib.h>

#include <list>

#include <stddef.h>

struct httpd_output;
struct page;

class HttpdClient final {
	/**
	 * The httpd output object this client is connected to.
	 */
	httpd_output *const httpd;

	/**
	 * The TCP socket.
	 */
	GIOChannel *channel;

	/**
	 * The GLib main loop source id for reading from the socket,
	 * and to detect errors.
	 */
	guint read_source_id;

	/**
	 * The GLib main loop source id for writing to the socket.  If
	 * 0, then there is no event source currently (because there
	 * are no queued pages).
	 */
	guint write_source_id;

	/**
	 * For buffered reading.  This pointer is only valid while the
	 * HTTP request is read.
	 */
	struct fifo_buffer *input;

	/**
	 * The current state of the client.
	 */
	enum {
		/** reading the request line */
		REQUEST,

		/** reading the request headers */
		HEADERS,

		/** sending the HTTP response */
		RESPONSE,
	} state;

	/**
	 * A queue of #page objects to be sent to the client.
	 */
	std::list<page *> pages;

	/**
	 * The #page which is currently being sent to the client.
	 */
	page *current_page;

	/**
	 * The amount of bytes which were already sent from
	 * #current_page.
	 */
	size_t current_position;

	/**
         * If DLNA streaming was an option.
         */
	bool dlna_streaming_requested;

	/* ICY */

	/**
	 * Do we support sending Icy-Metadata to the client?  This is
	 * disabled if the httpd audio output uses encoder tags.
	 */
	bool metadata_supported;

	/**
	 * If we should sent icy metadata.
	 */
	bool metadata_requested;

	/**
	 * If the current metadata was already sent to the client.
	 */
	bool metadata_sent;

	/**
	 * The amount of streaming data between each metadata block
	 */
	guint metaint;

	/**
	 * The metadata as #page which is currently being sent to the client.
	 */
	page *metadata;

	/*
	 * The amount of bytes which were already sent from the metadata.
	 */
	size_t metadata_current_position;

	/**
	 * The amount of streaming data sent to the client
	 * since the last icy information was sent.
	 */
	guint metadata_fill;

public:
	/**
	 * @param httpd the HTTP output device
	 * @param fd the socket file descriptor
	 */
	HttpdClient(httpd_output *httpd, int _fd, bool _metadata_supported);

	/**
	 * Note: this does not remove the client from the
	 * #httpd_output object.
	 */
	~HttpdClient();

	/**
	 * Frees the client and removes it from the server's client list.
	 */
	void Close();

	void LockClose();

	/**
	 * Returns the total size of this client's page queue.
	 */
	gcc_pure
	size_t GetQueueSize() const;

	/**
	 * Clears the page queue.
	 */
	void CancelQueue();

	bool Read();

	/**
	 * Data has been received from the client and it is appended
	 * to the input buffer.
	 */
	bool Received();

	/**
	 * Check if a complete line of input is present in the input
	 * buffer, and duplicates it.  It is removed from the input
	 * buffer.  The return value has to be freed with g_free().
	 */
	char *ReadLine();

	/**
	 * Handle a line of the HTTP request.
	 */
	bool HandleLine(const char *line);

	/**
	 * Switch the client to the "RESPONSE" state.
	 */
	void BeginResponse();

	/**
	 * Sends the status line and response headers to the client.
	 */
	bool SendResponse();

	gcc_pure
	int GetBytesTillMetaData() const;

	bool Write();

	/**
	 * Appends a page to the client's queue.
	 */
	void PushPage(page *page);

	/**
	 * Sends the passed metadata.
	 */
	void PushMetaData(page *page);
};

#endif
