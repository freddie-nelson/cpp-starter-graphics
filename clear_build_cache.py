import os

dir_path = os.path.dirname(os.path.realpath(__file__))
build_path = os.path.join(dir_path, "build")

os.system(f"cd {build_path} && meson configure --clearcache && meson setup --reconfigure")