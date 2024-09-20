import os

def check_exists(pathname):
   if not os.path.exists(pathname):
    print(f"{pathname} doesn't exist")
    exit(-1)

def replace_in_file(filepath, search_string, replace_string, enc='utf8'):
  if not os.path.exists(filepath):
     print(f"{filepath} doesn't exist")
     exit(-1)
  with open(filepath, 'r', encoding=enc) as file:
    content = file.read()
  new_content = content.replace(search_string, replace_string)
  with open(filepath, 'w', encoding=enc) as file:
    file.write(new_content)

# Example usage

solution_name = "HongLabAlgorithmsPart2"
check_exists(f"{solution_name}.sln")

old_name = input('Old project name: ')
check_exists(old_name)
check_exists(f"{old_name}/{old_name}.cpp")
check_exists(f"{old_name}/{old_name}.vcxproj.filters")
check_exists(f"{old_name}/{old_name}.vcxproj")

new_name = input('New project name: ')

# projname.cpp 파일의 이름 변경
os.rename(f"{old_name}/{old_name}.cpp", f"{old_name}/{new_name}.cpp")

# projname.vcxproj 내용 변경
replace_in_file(f"{old_name}/{old_name}.vcxproj", old_name, new_name)

# projname.vcxproj 파일의 이름 변경
os.rename(f"{old_name}/{old_name}.vcxproj", f"{old_name}/{new_name}.vcxproj")

# projname.vcxproj.filters 내용 변경
replace_in_file(f"{old_name}/{old_name}.vcxproj.filters", old_name, new_name)

# projname.vcxproj.filters 파일의 이름 변경
os.rename(f"{old_name}/{old_name}.vcxproj.filters", f"{old_name}/{new_name}.vcxproj.filters")

# projname 폴더 이름 변경
os.rename(f"{old_name}", f"{new_name}")

# solution_name.sln 내용 변경
replace_in_file(f"{solution_name}.sln", old_name, new_name)
