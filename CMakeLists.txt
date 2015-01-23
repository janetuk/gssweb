cmake_minimum_required(VERSION 2.8)

set(DIST_VERSION 0.1)

add_subdirectory(json_gssapi)
add_subdirectory(browsers)

add_custom_target(dist
	COMMAND git archive --prefix gssweb/ HEAD  |gzip -9 >gssweb-${DIST_VERSION}.tar.gz
)
