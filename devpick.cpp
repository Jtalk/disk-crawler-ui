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

#include <unistd.h>

#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <blkid/blkid.h>
#include <linux/fs.h>
#include <iostream>
bool get_size(const char *name, size_t &bytes) {
	int fd = open(name, O_RDONLY);
	bool success = ioctl(fd, BLKGETSIZE64 &bytes) >= 0;
	if (!success) {
		std::cout << name << " " << errno << std::endl;
	}
	close(fd);
	return success;
}

info_list_t devpick() {
	info_list_t result;
	blkid_cache cache;
	blkid_get_cache(&cache, nullptr);
	blkid_probe_all(cache);
	auto iterator = blkid_dev_iterate_begin(cache);
	blkid_dev device;
	while (blkid_dev_next(iterator, &device) == 0) {
		DeviceInfo info;
		auto name = blkid_dev_devname(device);
		info.size = 0;
		get_size(name, info.size);
		info.name = name;
		result.push_back(info);
	}
	blkid_dev_iterate_end(iterator);
	return result;
}
