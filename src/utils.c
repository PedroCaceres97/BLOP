#include <blop/utils.h>

const char* BlopUtilsPathLast(const char* path) {
    const char* last = path;

    last = strrchr(path, '\\');
    if (last != NULL) {
        path = strrchr(last, '\\');
        return blop_ternary(path != NULL, path, last);
    }

    last = strrchr(path, '/');
    if (last != NULL) {
        path = strrchr(last, '/');
        return blop_ternary(path != NULL, path, last);
    }

    return path;
}