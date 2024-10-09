
# BMP Image Conversion to YUV and Overlay on YUV Video
---

## Requirements:

- Windows 10 or higher
- MinGW compiler

---

## Instructions for Building and Running the Application:

### 1. Clone the Repository
```bash
git clone https://github.com/Devilartimus/BMP_to_YUV  
cd BMP_to_YUV
```

### 2. Build the Project via PowerShell or CMD
1. Open PowerShell or CMD in the project directory.
2. Run the following commands:
    
```bash
mkdir build
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
mingw32-make
```

3. Now the project is built and compiled using MinGW and GCC.
4. To run the executable file, use the command:

```bash
.\BMP_to_YUV.exe
```

5. If the terminal displays the message "Complete!", the program has run successfully.

---

## How to Use

### 1. Prepare BMP Image and YUV Videos
You will need:
- One BMP image
- Two YUV videos (one input video and one output video)

**Note:** The BMP file must have the same dimensions (in pixels) as the YUV video.

### 2. Modify Paths and Parameters in the Project
1. Open the `main.cpp` file and modify lines 9-11 as follows:

```cpp
string videoName = "Path_To_Input_Video/Video_Name.yuv";
string imageName = "Path_To_Input_Image/Image_Name.bmp";
string outputName = "Path_To_Output_Video/Video_Name.yuv";
```

2. Open the `Video.cpp` file and modify lines 42-43 as follows:

```cpp
_HEIGHT = Video_Height_in_Pixels;
_WIDTH = Video_Width_in_Pixels;
```

### 3. Rebuild and Run the Project
Rebuild the project as described in the build instructions, and then run the program again.
