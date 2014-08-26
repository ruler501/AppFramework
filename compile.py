#!/usr/bin/env python3

import platform
import subprocess
import os
import shutil
import sys
import glob

def buildAndroid():
	for f in glob.glob('src/*.cpp'):
		shutil.copy2(f, 'android/jni/src')
	os.chdir('android/jni/src')
	subprocess.call('ndk-build -j8', shell=True)
	os.chdir('../../')
	subprocess.call('ant debug install', shell=True)

def buildBlackberry():
	print("Not supported yet")
	
def buildCurrent():
	subprocess.call('codeblocks Framework.cbp --build')
	
def buildIOS():
	print("Please know that this will not work with Game Center curretnly")
	if platform.name().lower() != 'darwin':
		print("Not supported on non-OSX hosts")
		return False
	shutil.copytree('android/jni/SDL2', 'SDL2')
	os.chdir('SDL2/build-scripts')
	subprocess.call('sh iosbuild.sh', shell=True)
	os.chdir('../../')
	if not os.path.isdir('/Developer/Platforms/iOS.platform/Developer/Library/XCode/Project Templates/SDL2'):
		shutil.copytree('/Developer/Platforms/iOS.platform/Developer/Library/XCode/Project Templates/SDL2', 'SDL2/Xcode-iOS/Template/SDL iOS Application')
	print('Start a new project using the template /Developer/Platforms/iOS.platform/Developer/Library/XCode/Project Templates/SDL2.  The project should be immediately ready for use with SDL. Just add the source files in src')
	
def buildLinux():
	print("Not supported yet")
	
def buildOSX():
	print("Not supported yet")
	
def buildWindows():
	print("Not supported yet")
	
def buildWinPhone():
	print("Not supported yet")
	
builds = {'android' : buildAndroid, 'blackberry' : buildBlackberry, 'current' : buildCurrent, 'ios' : buildIOS,
		  'linux' : buildLinux, 'osx' : buildOSX, 'windows' : buildWindows, 'windowsPhone' : buildWinPhone}

def main(argv=None):
	if argv == None:
		argv = sys.argv
	if len(argv) != 2:
		print("Supply the os to build for")
		return False
	builds[sys.argv[1]]()
	return True

if __name__ == "__main__":
	main()