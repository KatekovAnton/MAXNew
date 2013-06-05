import os, fnmatch

import fnmatch
import os

matches = ""
dirs = ""
for root, dirnames, filenames in os.walk('../../MAX'):
  for filename in fnmatch.filter(filenames, '*.cpp'):
      if (root.find("cocos2d-2.0-x-2.0.4")==-1):
      	matches+=(os.path.join(root, filename))+" \\\n"
  for filename in fnmatch.filter(filenames, '*.h'):
      if (root.find("cocos2d-2.0-x-2.0.4")==-1) and (root.find("ios")==-1 and dirs.find(root)==-1):
      	dirs+="LOCAL_C_INCLUDES += $(LOCAL_PATH)/"+root+"\n"
print matches
print dirs