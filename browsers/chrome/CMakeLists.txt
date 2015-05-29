# CMakeLists for Chrome
cmake_minimum_required(VERSION 2.8)
if (DEFINED ENV{GSSWEB_CRX_PEM})
  if (WIN32)
    GET_FILENAME_COMPONENT(CHROME_ROOT_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Google Chrome;InstallLocation]" ABSOLUTE)
    set(BUILD_CRX COMMAND ${CHROME_ROOT_PATH}\\chrome.exe --pack-extension=${CMAKE_CURRENT_BINARY_DIR}\\gssweb --pack-extension-key=$ENV{GSSWEB_CRX_PEM})
  else (WIN32)
    set(BUILD_CRX COMMAND /usr/bin/chromium-browser --pack-extension=./gssweb --pack-extension-key=$ENV{GSSWEB_CRX_PEM})
  endif (WIN32)
endif (DEFINED ENV{GSSWEB_CRX_PEM})
add_custom_target( gssweb.crx ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/gssweb/background.js gssweb/background.js
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../common/contentscript.js gssweb/contentscript.js
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/gssweb/manifest.json gssweb/manifest.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../common/navigator.gssEap.js gssweb/navigator.gss.js
  ${BUILD_CRX}
)
