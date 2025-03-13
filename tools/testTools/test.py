import subprocess
import os
import platform
import logging
import sys

class SingleNewlineHandler(logging.StreamHandler):
    def emit(self, record):
        try:
            msg = self.format(record)
            self.stream.write(msg + '\n')
            self.flush()
        except Exception:
            self.handleError(record)

def configure():
    subprocess.run(
        [
            'cmake',
            '-B', 'build',
            '-G', 'Ninja',
            '-D', 'SPLITGUI_DEV=SPLITGUI_DEV'
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
    
def get_executables_in_directory(directory):
    executables = []
    
    if platform.system() == "Windows":
        exec_extensions = {".exe"}
    else:
        exec_extensions = set()

    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            
            if platform.system() == "Windows" and any(file.lower().endswith(ext) for ext in exec_extensions):
                executables.append(file_path)
            elif platform.system() != "Windows" and os.access(file_path, os.X_OK) and not file_path.lower().endswith(".so"):
                executables.append(file_path)
    
    return executables
    
def test():
    
    print('\n---- TESTS ----\n')
    
    build_dir = 'build/tests/'
    
    tests = get_executables_in_directory(build_dir)
    
    for test in tests:

        if len(sys.argv) > 1:
            if not test in sys.argv:
                continue
        
        test_path = test
        
        logging.info('')
        logging.info(f'---- {test_path} ----')
        logging.info('')
        
        try:
            result = subprocess.run(
                [
                    test_path
                ], 
                check=True,
                capture_output=True,
                text=True
            )
            
            if result.stdout:
                lines = result.stdout.splitlines()
                
                for line in lines:
                    if len(line) > 2:
                        if line[0] != '0' and line[1] != ':':
                            logging.debug(line)
                    else:
                        logging.debug(line)
                
            print(f'PASS: {test_path}')
            
        except subprocess.CalledProcessError as e:
            
            print(f'FAIL: {test_path}')
            
            logging.error(f'Error running {test_path}: {str(e)}')
            if e.stdout:
                lines = e.stdout.splitlines()
                
                for line in lines:
                    if len(line) > 2:
                        if line[0] != '0' and line[1] != ':':
                            logging.debug(line)
                    else:
                        logging.debug(line)
                    
            if e.stderr:
                lines = e.stderr.splitlines()
                
                for line in lines:
                    logging.error(line)
    
# -------------------------------- MAIN --------------------------------

if not os.path.isdir('src'):
    print('ERROR: call build script in project root dir')
    exit(-1)
    
if not os.path.isdir('build'):
    os.mkdir('build')
    
with open('build/tests.log', 'w'):
    pass
    
logging.basicConfig(
    filename='build/tests.log',
    level=logging.DEBUG,
    format='%(message)s'
)
    
logging.info('--- BEGIN LOG ---')

configure()
build()
test()

logging.info('')
logging.info('--- END LOG ---')