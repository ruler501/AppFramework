import platform
import subprocess
import os
import shutil
import sys

def buildAndroid():
	for f in glob.glob('src/*.cpp'):
		shutil.copy2(f, 'android/jni/src')
	os.chdir('android/jni/src')
	subprocesss.call('ndk-build -j8', shell=True)
	os.chdir('../../')
	subproces.call('ant debug install', shell=True)

def buildBlackberry():
	print("Not supported yet")
	
def buildCurrent():
	subprocess.call('codeblocks Framework.cbp --build'
	
def buildIOS():
	print("Not supported yet")
	
def buildLinux():
	print("Not supported yet")
	
def buildOSX():
	print("Not supported yet")
	
def buildWindows():
	print("Not supported yet")
	
def buildWinPhone():
	
builds = {'android' : buildAndroid, 'blackberry' : buildBlackberry, 'current' : buildCurrent, 'ios' : buildIOS,
		  'linux' : buildLinux, 'osx' : buildOSX, 'windows' : buildWindows, 'windowsPhone' : buildWinPhone}

def main(argv=None):
	if argv == None
		argv = sys.argv
	if len(argv) != 2:
		print("Supply the os to build for")
		return False
	builds[sys.argv[1]]()
	return True

if __name__ == "__main__":
	main()