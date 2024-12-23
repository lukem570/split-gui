import subprocess
import os
import shutil

def configure():
    subprocess.run(
        [
            'cmake',
            '-B', 'build',
            '-G', 'Ninja'
        ],
        check=True,
        text=True
    )
    
def build():
    subprocess.run(
        [
            'ninja'
        ],
        cwd='build',
        check=True,
        text=True
    )
    
def structure():
    if not os.path.isdir('lib'):
        os.mkdir('lib')
    
    if not os.path.isdir('lib/bin'):
        os.mkdir('lib/bin')
    
    shutil.copytree('src/include', 'lib/include')
    shutil.copy('build/libsplitgui.dll', 'lib/bin/libsplitgui.dll')
    
# -------------------------------- MAIN --------------------------------

if not os.path.isdir('src'):
    print('ERROR: call build script in project root dir')
    exit(-1)
    
if not os.path.isdir('build'):
    os.mkdir('build')

configure()
build()
structure()