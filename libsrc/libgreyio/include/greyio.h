/*
 * Copyright (c) 2014 Damian Jason Lapidge
 *
 * The contents of this file are subject to the terms and conditions defined 
 * within the file LICENSE.txt, located within this project's root directory.
 */

#ifndef GREYIO_H
#define GREYIO_H

ssize_t readall(int fd, void **buf);
ssize_t writeall(int fd, void *buf, size_t count);

#endif /* ifndef GREYIO_H */

