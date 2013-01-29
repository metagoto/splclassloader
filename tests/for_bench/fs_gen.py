#!/usr/bin/python

# generate a dummy subtree filed with php files. each file declares one blank class.
# also generates a master php file which instanciates all classes in a row.   
# the only purpose is to give something to eat to php and launch apache bench. 
# args:
# 
# --prefix=/path/to/dir
#   default: ./lib
#   does not override an existing dir 
#
# --out=/path/to/file.php
#   default: ./batch_instances.php
#   does not override an existing file
#
# --depth
#   default: 2
#   max: 3
#   number of dummy 'vendor' namespaces and sub namespaces. recursive
#


import sys, os
from optparse import OptionParser


MAX_DEPTH = 3
DEF_DEPTH = 2
DEF_PREFIX= './lib'
DEF_OUT   = './batch_instances.php'


def ns_conc(a,b) : return a + '\\' + b
def dir_conc(a,b) : return a + os.sep + b

def gen(depth, index, ns, cur_depth, items) :
    if cur_depth > depth :
        return
    lns = ns[:]
    lns[-1] = lns[-1] + `index`
    
    #### be creative
    items.append(['AA' ,      lns])
    items.append(['AAA' ,     lns])
    items.append(['AAAA' ,    lns])
    items.append(['AAAAA' ,   lns])
    items.append(['AAAAAA' ,  lns])
    items.append(['B_B',      lns])
    items.append(['B_BB',     lns])
    items.append(['B_BBB',    lns])
    items.append(['B_BBBB',   lns])
    items.append(['B_B_B',    lns])
    items.append(['B_B_BB',   lns])
    #items.append(['Final_Abstract_Singleton_Interface', lns])
    ####
    
    for i in range(depth) :
        gen(depth, i, lns + ['sub'], cur_depth+1, items)



def gen_content(cl, ns) :
    s = """<?php
namespace %s;
class %s { }
""" % (reduce(ns_conc, ns), cl)
    return s



def write_file(prefix, item, str) :
    cl = item[0]
    cl_tok = cl.split('_')
    path = os.path.join(prefix, reduce(os.path.join, item[1]));
    if len(cl_tok) > 1 :
        path = os.path.join(path, reduce(dir_conc, cl_tok[:-1]))
        cl = cl_tok[-1]
    
    file = path + os.sep + cl + '.php'
    if not os.path.exists(path) :
        os.makedirs(path)
    f = open(file, 'w')
    f.write(str)
    f.close()



##############

parser = OptionParser()
parser.add_option("--prefix")
parser.add_option("--out")
parser.add_option("--depth", type="int")
(options, args) = parser.parse_args()


depth = options.depth if options.depth else DEF_DEPTH
if depth > MAX_DEPTH :
    print 'error: depth cannot be greater than %d' % MAX_DEPTH
    sys.exit()


out = options.out if options.out else DEF_OUT
if os.path.exists(out) :
    print 'error: file "%s" already exists' % out
    sys.exit()

        
prefix = options.prefix if options.prefix else DEF_PREFIX
if os.path.exists(prefix) :
    print 'error: directory "%s" already exists' % prefix
    sys.exit()

print 'creating directory "%s"' % prefix
os.mkdir(prefix)
   
if not os.path.exists(prefix) :
   print 'error: cannot create directory "%s"' % prefix
   sys.exit() 



##############

items = []

for i in range(depth) :
    gen(depth, i, ['vendor'], 0, items)
    
buf = "<?php\n"
for i in items :
    write_file(prefix, i, gen_content(i[0], i[1]))
    buf += "$a = new \\%s\\%s;\n" % (reduce(ns_conc, i[1]), i[0])
    

f = open(out, 'w')
f.write(buf)
f.close()

print 'done. out file is "%s", subtree is under "%s"' % (out, prefix)
