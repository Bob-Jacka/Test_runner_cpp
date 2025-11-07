"""
Script for cmake file.
Find all paths to files with cpp or hpp extensions.
"""

import os

files_path: list[str] = []
exclude_file_names: list[str] = ['.gitignore',
                                 'README.md',
                                 'External Libraries',
                                 'Scratches and Consoles',
                                 'DependenciesManager.py',
                                 'CMakeLists.txt',
                                 '.venv',
                                 'cmake-build-debug',
                                 '.git']
"""
Which filenames to exclude from search
"""


def __contains_one_of_exclude(name: str) -> bool:
    """
    Function for checking that filename do not contain in exclude list.
    :param name: name for checking
    :return: boolean result
    """
    if name is not None:
        for exclude_fragment in exclude_file_names:
            if exclude_fragment in name:
                return True
    return False


def __get_filename_from_path(path: str):
    """
    Function for receiving filename from given path,
    :param path: given path
    :return: file name
    """
    cond = os.path.sep in path
    if cond:
        return path[path.rfind(os.path.sep):]
    else:
        return path


def main_cycle(path: str):
    file_name = __get_filename_from_path(path)
    if not __contains_one_of_exclude(file_name):
        if os.path.isfile(path):
            files_path.append(path)
        elif os.path.isdir(path):
            for entry in os.listdir(path):
                main_cycle(os.path.join(path, entry))
        else:
            print(f"{path}: neither file nor dir")
    else:
        print(f'Name of the element "{file_name}" in exclude list!')


def print_paths(prefix: str):
    for file in files_path:
        changed_filename = file.removeprefix(prefix)
        changed_filename = changed_filename.replace('\\', '/')
        if not changed_filename.startswith('.'):
            print(changed_filename)


if __name__ == '__main__':
    main_cycle('.')  # get files from current directory
    print('####################################', end='\n\n')
    print_paths('.\\')
