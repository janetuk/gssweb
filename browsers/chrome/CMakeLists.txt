# Copyright (c) 2014, 2015 JANET(UK)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of JANET(UK) nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.

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
