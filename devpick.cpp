/*
 *  Disk Crawler library.
 *  Copyright (C) 2013  Jtalk <me@jtalk.me>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "devpick.h"

#include <blkid/blkid.h>
#include <iostream>
info_list_t devpick() {
	info_list_t result;
	blkid_cache cache;
	blkid_get_cache(&cache, nullptr);
	blkid_probe_all(cache);
	auto iterator = blkid_dev_iterate_begin(cache);
	blkid_dev device;
	while (blkid_dev_next(iterator, &device) == 0) {
		DeviceInfo info;
		info.name = blkid_dev_devname(device);
		info.size = 0;
		result.push_back(info);
	}
	blkid_dev_iterate_end(iterator);
	return result;
}
