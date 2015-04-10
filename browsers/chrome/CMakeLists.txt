# CMakeLists for Chrome
cmake_minimum_required(VERSION 2.8)

add_custom_target( gssweb.crx ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/gssweb/background.js gssweb/background.js
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../common/contentscript.js gssweb/contentscript.js
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/gssweb/manifest.json gssweb/manifest.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../common/navigator.gssEap.js gssweb/navigator.gssEap.js
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../../chrome.pem chrome.pem
  COMMAND /usr/bin/chromium-browser --pack-extension=./gssweb --pack-extension-key=./chrome.pem 
)
