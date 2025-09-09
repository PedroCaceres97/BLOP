target_sources(blop PRIVATE blop.c)
target_sources(blop PRIVATE utils.c)

add_subdirectory(modules)
add_subdirectory(containers)