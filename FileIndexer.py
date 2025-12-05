"""
Script for cmake file in C++.
Find all paths to files with cpp or hpp extensions.
"""

import os

files_path: list[str] = []
exclude_file_names: list[str] = ['.gitignore',
                                 'README.md',
                                 'External Libraries',
                                 'Scratches and Consoles',
                                 'FileIndexer.py',
                                 'CMakeLists.txt',
                                 '.venv',
                                 'cmake-build-debug',
                                 '.git']
"""
Which filenames to exclude from search
"""

CMAKE_SEP: str = '/'
WINDOWS_SEP: str = '\\'
LINUX_SEP: str = '/'


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
    """
    Proceed filenames into list
    :param path: path to search for files
    :return: None
    """
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
    """
    Print all paths starting with given prefix
    :param prefix:
    :return: None
    """
    current_sep = os.name == 'posix' if LINUX_SEP else WINDOWS_SEP
    for file in sorted(files_path):
        changed_filename = file.removeprefix(prefix)
        changed_filename = changed_filename.replace(current_sep, CMAKE_SEP)
        if not changed_filename.startswith('.'):
            print(changed_filename)


if __name__ == '__main__':
    main_cycle('.')  # get files from current directory
    print('####################################', end='\n\n')
    print_paths('.\\')
