#
# Copyright 2016 The Imaging Source Europe GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.




LIBS:=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config gstreamer-1.0 --libs)
LIBS:=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config gstreamer-video-1.0 --libs)
LIBS+=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config gobject-introspection-1.0 --libs)

# to build within the source tree
# enable the following lines
# and source build directory ./env.sh
# LIBS+=-L./../../build/libs

CFLAGS:=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config gstreamer-1.0 --cflags)
CFLAGS:=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config gstreamer-video-1.0 --cflags)
CFLAGS+=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config gobject-introspection-1.0 --cflags)

# CFLAGS+=-I../../libs/tcam-property/src
# CFLAGS+=-I../../src/gstreamer-1.0/tcamsrc

.PHONY: all clean

all: demo

demo:  demo.c
	$(CC) -g -DGST_DISABLE_DEPRECATED -Wall -Wextra -O0 $(CFLAGS) -o $@ demo.c $(LIBS)
	mkdir -p images
clean:
	$(RM) *.o
	rm -rf images/
	$(RM) demo
