# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

import os 
import sys
project = sys.argv[1]
path_in = sys.argv[2]
path_out = sys.argv[3]

import json

#
#   Contributors
#

enum_h = "#undef GEN_CONTRIBUTIONS\n"
enum_h += "#undef GEN_LIBRARIES\n"
enum_h += "#define GEN_CONTRIBUTIONS { "

print("~~");
    
enum_list = []

with open(path_in + "/contributors.json") as f:
    contributors = json.loads(f.read())["contributors"]
    
first = 1
for elem in contributors:
    if not first:
        enum_h += ", "
    enum_h += '"' + elem["name"] + '"'
    enum_list.append(elem["name"])
    first = 0
    
enum_h += " }"
    
if not first:
    enum_list.sort();
    print("~~ " + project + " has been contributed to by\n~~  " + ", ".join(enum_list));

#
#   Libraries
#

enum_h += "\n#define GEN_LIBRARIES { "
    
enum_list = []

with open(path_in + "/libraries.json") as f:
    libraries = json.loads(f.read())["libraries"]

first = 1
for elem in libraries:
    elem_enum_name = elem["name"];

    if not first:
        enum_h += ", "
    enum_h += ' { "' + elem["name"] + '"'
    if 'creator' in elem:
        enum_h += ', "' + elem["creator"] + '"'
        elem_enum_name += " (" + elem["creator"] + ")"
    else:
        enum_h += '"'
    if 'url' in elem:
        enum_h += ', "' + elem["url"] + '"'
    else:
        enum_h += '"'
    enum_h += " }"
    
    enum_list.append(elem_enum_name);
    
    first = 0
    
enum_h += " }"
    
if not first:
    enum_list.sort();
    print("~~ And is built using \n~~  " + ", ".join(enum_list));
    
print("~~")
    
with open(path_out + "/def_contribute.h", "w") as f:
    f.write(enum_h)