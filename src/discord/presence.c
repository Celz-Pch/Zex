/*
** Cz PROJECT, 2026
** presence
** File description:
** Discord Rich Presence via Unix IPC socket
*/

#include "zex.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DISCORD_CLIENT_ID "1475274360083185714"
#define OP_HANDSHAKE      0
#define OP_FRAME          1

static int discord_connect(void)
{
    const char *runtime;
    char path[256];
    struct sockaddr_un addr;
    struct timeval tv;
    int fd;
    int i;

    runtime = getenv("XDG_RUNTIME_DIR");
    if (!runtime)
        return -1;
    for (i = 0; i < 10; i++) {
        snprintf(path, sizeof(path), "%s/discord-ipc-%d", runtime, i);
        fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd < 0)
            continue;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
        if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            return fd;
        }
        close(fd);
    }
    return -1;
}

static int discord_send(int fd, uint32_t opcode, const char *json)
{
    uint32_t len = (uint32_t)strlen(json);
    uint8_t header[8];

    header[0] = opcode & 0xFF;
    header[1] = (opcode >> 8) & 0xFF;
    header[2] = (opcode >> 16) & 0xFF;
    header[3] = (opcode >> 24) & 0xFF;
    header[4] = len & 0xFF;
    header[5] = (len >> 8) & 0xFF;
    header[6] = (len >> 16) & 0xFF;
    header[7] = (len >> 24) & 0xFF;
    if (write(fd, header, 8) != 8)
        return -1;
    if (write(fd, json, len) != (ssize_t)len)
        return -1;
    return 0;
}

static void discord_recv(int fd)
{
    uint8_t header[8];
    uint32_t len;
    char *buf;
    ssize_t n;

    n = recv(fd, header, 8, 0);
    if (n != 8)
        return;
    len = (uint32_t)header[4] | ((uint32_t)header[5] << 8)
        | ((uint32_t)header[6] << 16) | ((uint32_t)header[7] << 24);
    if (len == 0 || len > 65536)
        return;
    buf = malloc(len + 1);
    if (!buf)
        return;
    recv(fd, buf, len, 0);
    free(buf);
}

void discord_init(editor_t *ed)
{
    char json[256];

    ed->discord_fd = discord_connect();
    if (ed->discord_fd < 0)
        return;
    ed->discord_start = (long)time(NULL);
    snprintf(json, sizeof(json),
        "{\"v\":1,\"client_id\":\"%s\"}", DISCORD_CLIENT_ID);
    if (discord_send(ed->discord_fd, OP_HANDSHAKE, json) < 0) {
        close(ed->discord_fd);
        ed->discord_fd = -1;
        return;
    }
    discord_recv(ed->discord_fd);
}

void discord_update(editor_t *ed)
{
    char details_buf[128];
    char json[1024];
    const char *details;
    const char *base;

    if (ed->discord_fd < 0)
        return;
    if (ed->filename) {
        base = strrchr(ed->filename, '/');
        base = base ? base + 1 : ed->filename;
        snprintf(details_buf, sizeof(details_buf), "Editing: %s", base);
        details = details_buf;
    } else if (ed->has_tree) {
        details = "Browsing files";
    } else {
        details = "No file open";
    }
    snprintf(json, sizeof(json),
        "{\"cmd\":\"SET_ACTIVITY\","
        "\"args\":{\"pid\":%d,"
        "\"activity\":{"
        "\"details\":\"%s\","
        "\"state\":\"Line %d | %d lines\","
        "\"timestamps\":{\"start\":%ld},"
        "\"assets\":{"
        "\"large_image\":\"zex\","
        "\"large_text\":\"Zex Editor\""
        "}}},"
        "\"nonce\":\"zex1\"}",
        (int)getpid(), details, ed->y + 1, ed->num_lines,
        ed->discord_start);
    if (discord_send(ed->discord_fd, OP_FRAME, json) < 0) {
        close(ed->discord_fd);
        ed->discord_fd = -1;
        return;
    }
    recv(ed->discord_fd, json, sizeof(json), MSG_DONTWAIT);
}

void discord_shutdown(editor_t *ed)
{
    if (ed->discord_fd >= 0) {
        close(ed->discord_fd);
        ed->discord_fd = -1;
    }
}