call "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Visual Studio 2017\Visual Studio Tools\VC\x64 Native Tools Command Prompt for VS 2017 (3)"
cd /d C:\The-Powder-Toy&meson -Dbuildtype=release -Dstatic=prebuilt -Db_vscrt=static_from_buildtype build-release-static&cd build-release-static&cls&ninja
cmd /k