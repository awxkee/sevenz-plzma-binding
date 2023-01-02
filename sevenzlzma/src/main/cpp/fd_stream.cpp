//
// Created by Radzivon Bartoshyk on 02/01/2023.
//

#include "fd_stream.h"
#include <sys/stat.h>
#include <memory>
#include <unistd.h>

void deinit_fd_context(void *context) {
    free(context);
}

plzma_context create_fd_context(int fd) {
    auto ctx = reinterpret_cast<FdStream *>(malloc(sizeof(FdStream)));
    ctx->fd = fd;
    return plzma_context{ctx, deinit_fd_context};
}

bool fd_open_callback(void *context) {
    return true;
}

void fd_close_callback(void *context) {

}

bool fd_seek_callback(void *context,
                      int64_t offset,
                      uint32_t seek_origin,
                      uint64_t *new_position) {
    auto ctx = reinterpret_cast<FdStream *>(context);
    auto finalOffset = lseek64(ctx->fd, offset, (int) seek_origin);
    if (finalOffset == -1) {
        return false;
    }
    if (new_position) {
        *new_position = finalOffset;
    }
    return true;
}

bool fd_read_callback(void *context,
                      void *data,
                      uint32_t size,
                      uint32_t *processed_size) {
    auto ctx = reinterpret_cast<FdStream *>(context);
    auto readSize = pread64(ctx->fd, data, size, lseek(ctx->fd, 0, SEEK_CUR));
    if (readSize == -1) {
        return false;
    }
    if (processed_size) {
        *processed_size = readSize;
    }
    return true;
}