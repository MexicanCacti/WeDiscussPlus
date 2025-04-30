# WeDiscuss++

## How to Build
1. Clone the repository:
```bash
git clone https://github.com/MexicanCacti/WeDiscussPlus.git
```

2. Navigate to the build/temp directory:
```bash
cd WeDiscussPlus
cd build/temp
```

3. Generate build files using CMake:
```bash
cmake ../..
```

4. Build the project:
```bash
cmake --build .
```

5. Executables located in:
```bash
build/bin/Debug/
```
**Note** If building in Release, the executables will be located in:
```bash
build/bin/Release/
```

## Set up Git Hooks
```bash
git config core.hooksPath WeDiscussPlus/githooks
```