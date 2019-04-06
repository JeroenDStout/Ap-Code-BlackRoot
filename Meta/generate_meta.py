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
    if not first:
        enum_h += ", "
    enum_h += '"' + elem["name"] + '"'
    enum_list.append(elem["name"])
    first = 0
    
enum_h += " }"
    
if not first:
    enum_list.sort();
    print("~~ And is built using \n~~  " + ", ".join(enum_list));
    
print("~~")
    
with open(path_out + "/def_contribute.h", "w") as f:
    f.write(enum_h)