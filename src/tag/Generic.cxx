/*
 * Copyright 2003-2017 The Music Player Daemon Project
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

#include "config.h"
#include "Generic.hxx"
#include "Id3Scan.hxx"
#include "ApeTag.hxx"
#include "fs/Path.hxx"
#include "thread/Mutex.hxx"
#include "input/InputStream.hxx"
#include "input/LocalOpen.hxx"
#include "Log.hxx"

#include <exception>

bool
ScanGenericTags(InputStream &is, TagHandler &handler) noexcept
{
	if (tag_ape_scan2(is, handler))
		return true;

#ifdef ENABLE_ID3TAG
	try {
		is.LockRewind();
	} catch (...) {
		return false;
	}

	return tag_id3_scan(is, handler);
#else
	return false;
#endif
}

bool
ScanGenericTags(Path path, TagHandler &handler) noexcept
try {
	Mutex mutex;

	auto is = OpenLocalInputStream(path, mutex);
	return ScanGenericTags(*is, handler);
} catch (...) {
	LogError(std::current_exception());
	return false;
}
