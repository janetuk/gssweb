cmake_minimum_required(VERSION 2.8)

add_custom_target( gssweb.xpi ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/packaging/locales.json locales.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lib/main.js resources/gssweb/lib/main.js
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../common/contentscript.js resources/gssweb/data/contentscript.js
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/packaging/install.rdf install.rdf
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/packaging/harness-options.json harness-options.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/packaging/bootstrap.js bootstrap.js
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/packaging/defaults/preferences/prefs.js defaults/preferences/prefs.js
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../common/navigator.gssEap.js chrome/content/navigator.gssEap.js
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/chrome.manifest chrome.manifest
  COMMAND zip gssweb.xpi locales.json resources/gssweb/lib/main.js ./resources/gssweb/data/contentscript.js ./install.rdf ./harness-options.json ./bootstrap.js ./defaults/preferences/prefs.js ./chrome/content/navigator.gssEap.js ./chrome.manifest
)
