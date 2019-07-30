# PNG2DAE-GUI
Graphical User Interface for console built version of PNG2DAE utility

# Setup
The project is standalone and does not require any Qt libraries. 
## Requirements
- Qt 5.13.0 (source-based static assembly)
## Building
```
git clone https://github.com/kmpchk/PNG2DAE-GUI png2daegui
cd png2daegui
mkdir build
cd build
qmake ../Source/png2dae_gui.pro -spec linux-g++
make
```
## Open an executable file
```
./png2dae_gui
```
