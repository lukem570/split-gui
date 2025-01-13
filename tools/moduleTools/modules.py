import os
import shutil
import subprocess
import requests
import zipfile
import platform

def fetch_source(repos):
    for repo in repos:
        subprocess.run(
            [
                'git', 'clone', '--recursive', repo
            ],
            cwd='include/src',
            text=True
        )

def fetch_vulkan_sdk(url):
    sdk_zip_path = "include/src/vulkan-sdk/vulkan_sdk.zip"

    response = requests.get(url)
    with open(sdk_zip_path, 'wb') as file:
        file.write(response.content)
    
    with zipfile.ZipFile(sdk_zip_path, 'r') as zip_ref:
        zip_ref.extractall("include/src/vulkan-sdk")

def build_glfw():
    subprocess.run(
        [
            'cmake', 
            '-S', 'include/src/glfw', 
            '-B', 'include/src/glfw/build',
            '-G', 'Ninja'
        ],
        check=True,
        text=True
    )
    
    subprocess.run(
        [
            'ninja'
        ],
        cwd='include/src/glfw/build',
        check=True,
        text=True
    )

def structure():
    shutil.copytree('include/src/glfw/include/GLFW', 'include/GLFW')
    shutil.copytree('include/src/glfwpp/include/glfwpp', 'include/glfwpp')
    shutil.copytree('include/src/Vulkan-Headers/include/vulkan', 'include/vulkan')
    shutil.copytree('include/src/Vulkan-Headers/include/vk_video', 'include/vulkan/vk_video')
    shutil.copytree('include/src/Vulkan-Hpp/vulkan', 'include/vulkan', dirs_exist_ok=True)
    shutil.copytree('include/src/Vulkan-Hpp/vulkan', 'include/vulkan', dirs_exist_ok=True)
    
    shutil.copy('include/src/vulkan-sdk/VulkanRT-1.3.296.0-Components/x64/vulkan-1.dll', 'build/vulkan-1.dll')
    shutil.copy('include/src/glfw/build/src/libglfw3.a', 'build/libglfw3.a')
  
# -------------------------------- MAIN --------------------------------

if not os.path.isdir('build'):
    os.mkdir('build')
    
if not os.path.isdir('include'):
    os.mkdir('include')
    
if not os.path.isdir('include/src'):
    os.mkdir('include/src')

if not os.path.isdir('include/src/vulkan-sdk'):
    os.mkdir('include/src/vulkan-sdk')

fetch_source(
    [
        'https://github.com/glfw/glfw.git',
        'https://github.com/KhronosGroup/Vulkan-Hpp.git',
        'https://github.com/KhronosGroup/Vulkan-Headers.git', 
        'https://github.com/lukem570/glfwpp.git',
        'https://github.com/zeux/volk.git', 
        'https://github.com/Chlumsky/msdfgen.git', # cmake -G "Ninja" -DMSDFGEN_CORE_ONLY=ON -B build
        'https://github.com/freetype/freetype.git', # cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -B build 
    ]
)

# windows
if platform.system() == 'Windows':
    fetch_vulkan_sdk('https://sdk.lunarg.com/sdk/download/1.3.296.0/windows/VulkanRT-1.3.296.0-Components.zip')

build_glfw()
structure()