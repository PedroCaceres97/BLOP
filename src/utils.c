#include <blop/utils.h>

const char* BlopUtilsPathLast(const char* path) {
    const char* last = path;

    last = strrchr(path, '\\');
    if (last != NULL) {
        path = strrchr(last, '\\');
        return ternary(path, last, path != NULL);
    }

    last = strrchr(path, '/');
    if (last != NULL) {
        path = strrchr(last, '/');
        return ternary(path, last, path != NULL);
    }

    return path;
}