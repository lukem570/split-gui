import subprocess
import os
import logging

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
    
def test():
    
    print('\n---- TESTS ----\n')
    
    build_dir = 'build/build/'
    
    tests = [f for f in os.listdir(build_dir) if f.endswith('.exe')]
    
    for test in tests:
        
        test_path = os.path.join(build_dir, test)
        
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
                    logging.debug(line)
                
            print(f'PASS: {test_path}')
            
        except subprocess.CalledProcessError as e:
            
            print(f'FAIL: {test_path}')
            
            logging.error(f'Error running {test_path}: {str(e)}')
            if e.stdout:
                lines = e.stdout.splitlines()
                
                for line in lines:
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
    format='%(asctime)s\t- %(levelname)s\t- %(message)s'
)
    
logging.info('--- BEGIN LOG ---')

configure()
build()
test()

logging.info('')
logging.info('--- END LOG ---')