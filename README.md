
![Thumbnail](./assets/Thumbnail.png)

# 🚀 ASCII ART GENERATOR

> A small ascii generator I did to learn a bit of c

---

## 📝 Introduction

This is a small project to get an introduction into the quirks of c. Comming from a Java background it helped me understand basic topics like pointers and their behaviour with functions (as parameters or return values).

## ⚙️ How it works

The project itself is nothing crazy, it uses a library to convert common image formats into a bitmap, then it is resized to a smaller size. After that it selects a character to represent each pixel based on the brightness and calculates the ANSII color that best matches the color of each pixel. In the end it prints the result on the console with color.

This is the repo with the libraries used here and much more:
https://github.com/nothings/stb/tree/master  

---
## 🔧 How to install
1. **Clone the repo**
To clone the repo execute the following command on the container folder
   ```bash
   git clone [https://github.com/ChristianLuci/AsciiArtGenerator](https://github.com/ChristianLuci/AsciiArtGenerator)
   ```
Then compile the project using the following command
  ```bash
  gcc src/imageConverter.c src/stb_image_impl.c src/ascii_generation_impl.c -o "imageConverter" -lm
   ```
This will generate the executable on the root folder
## 🚀 How to execute

Execute the file with the following flags:

- `-help` -> Displays the available parameters.
- `-i` -> Specifies the image to use.
- `-w` [optional] -> Specifies the width of the console output (default is 300 pixels). The height uses the same proportions as the original image.


## 📌 Execution examples
Command:    
   ```bash
   imageConverter -i "image/example.png" -w 375
   ```
Result:
    ![Example result](./assets/Example.png)
