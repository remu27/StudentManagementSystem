모든 파일은 UTF-8이 아닌 EUC-KR로 인코딩되어 있습니다. 파일이 깨져보일 시에 파일 인코딩을 EUC-KR로 바꾸어 주시길 바랍니다.

프로그램 실행은 ./student_system.exe 를 터미널에 입력시키면 됩니다.

Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
choco install mingw