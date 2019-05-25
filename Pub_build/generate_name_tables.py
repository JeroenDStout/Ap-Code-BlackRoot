# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Additionally, all Source Code Forms generated using this form are
# themselves inherently subject to the Mozilla Public Licence, v. 2.0.

import sys
import ntpath
import os
import json
import unidecode
import random
from datetime import datetime
import argparse

sys.path.append(os.path.join(os.path.dirname(__file__), '../pub_py'))
import hash_id

parser = argparse.ArgumentParser(description='Compile table file into language-specific formats')
parser.add_argument('-input', help='Table input file', required=True)
parser.add_argument('-output_dir', help='Output directory', required=True)
parser.add_argument('-namespace', help='Add namespace (separations with ::)')

args = parser.parse_args(sys.argv[1:])

    # Obtain the relevant paths
    
path_in       = args.input
path_dir, path_file = ntpath.split(path_in)
path_dir_base = args.output_dir
path_file_raw = (os.path.splitext(path_file)[0])

split_namespace = args.namespace.split('::')

try:
    os.makedirs(path_dir_base + "/-genc/")
except FileExistsError:
    pass
path_out_cpp = path_dir_base + "/-genc/" + path_file_raw + ".h"

try:
    os.makedirs(path_dir_base + "/-gen_py/")
except FileExistsError:
    pass
path_out_py = path_dir_base + "/-gen_py/" + path_file_raw + ".py"

try:
    os.makedirs(path_dir_base + "/-genex/")
except FileExistsError:
    pass
path_out_json = path_dir_base + "/-genex/" + path_file_raw + ".json"

    # Read file
    
with open (path_in, "r") as myfile:
    raw_data = myfile.read()
    
raw_tables = list(filter(None, raw_data.split("#Table")))
full_tables = []

    # Create tables
    
for table in raw_tables:
    data = list(filter(None, table.split("#Names")))
    table_name, table_size = filter(None, data[0].split())
    table_list = list(set(filter(None, data[1].split("\n"))))
    cleaned_list = []
    
    min_length = 999
    max_length = 0
    
    for name in table_list:
        clean_name = unidecode.unidecode(" ".join(w.capitalize() for w in name.strip().split()))
    
        if len(clean_name) < 2:
          continue
        if len(clean_name) > 15:
          continue
        
        min_length = min(min_length, len(clean_name))
        max_length = max(max_length, len(clean_name))
          
        cleaned_list.append(clean_name)
        
    table_list = sorted(cleaned_list)
    random.Random(1234).shuffle(table_list)
    
    table_list = table_list[0:int(table_size)]
    
    full_tables.append({
      "name" : table_name,
      "list" : table_list,
      "min"  : min_length,
      "max"  : max_length
    })
    print(" " + table_name + " (" + str(len(table_list)) + ")")
    
    # Output
    
with open(path_out_json, "w") as text_file:
    text_file.write(json.dumps(full_tables, indent=2))
    
with open(path_out_cpp, "w") as text_file:
    text_file.write('''\
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
 
    /*~*~*~*~  Auto Generated on ''' + datetime.now().strftime("%Y/%m/%d, %H:%M:%S") + '''  ~*~*~*~*/
    /*                                                        */
    /* Please be adviced that editing this file is pointless. */
    /*                                                        */
    /*~*~*~*~                                          ~*~*~*~*/
 
#pragma once

namespace ''' + " {\nnamespace ".join(split_namespace) + ''' {

''')

    for table in full_tables:
        text_file.write('    namespace ' + table['name'] + ' {\n')
        
        text_file.write('        size_t const count = ' + str(len(table['list'])) + ';\n')
        text_file.write('        size_t const min_length = ' + str(table['min']) + ';\n')
        text_file.write('        size_t const max_length = ' + str(table['max']) + ';\n')
        text_file.write('        char * const table[' + str(len(table['list'])) + '] = {\n          "' + '", "'.join(table['list']) + '"\n        };\n')
        text_file.write('\n')
        text_file.write('        constexpr char * const get(size_t i) {\n')
        text_file.write('            return table[i % ' + str(len(table['list'])) + '];\n')
        text_file.write('        }\n')
        
        text_file.write('    };\n\n')

    text_file.write('''\
''' + "}\n" * len(split_namespace) + '''
''')