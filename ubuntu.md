### dpkg: warning: files list file for package 'x' missing

> https://serverfault.com/questions/430682/dpkg-warning-files-list-file-for-package-x-missing?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa

```
for package in $(apt-get upgrade 2>&1 |\
                 grep "warning: files list file for package '" |\
                 grep -Po "[^'\n ]+'" | grep -Po "[^']+"); do
    apt-get install --reinstall "$package";
done
```
