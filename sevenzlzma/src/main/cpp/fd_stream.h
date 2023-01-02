//
// Created by Radzivon Bartoshyk on 02/01/2023.
//

#ifndef SEVEN7LZMA_FD_STREAM_H
#define SEVEN7LZMA_FD_STREAM_H

#include <cstdint>
#include "libplzma.hpp"

struct FdStream {
    int fd;
};

plzma_context create_fd_context(int fd);

bool fd_open_callback(void *context);

void fd_close_callback(void *context);

bool fd_seek_callback(void *context,
                      int64_t offset,
                      uint32_t seek_origin,
                      uint64_t *new_position);

bool fd_read_callback(void *context,
                      void *data,
                      uint32_t size,
                      uint32_t *processed_size);

#endif //SEVEN7LZMA_FD_STREAM_H
