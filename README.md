# 🔐 Puzzle Lock

![C](https://img.shields.io/badge/C-Language-blue)
![CLI](https://img.shields.io/badge/Interface-Command%20Line-green)
![License](https://img.shields.io/badge/License-MIT-brightgreen)

Puzzle Lock is a small interactive terminal game written in C.  
The goal is to guess a hidden 3-digit lock code using hints after each attempt.

---

## 📖 Overview

This project is a simple logic-based guessing game inspired by lock-code puzzles.

The program randomly generates a 3-digit secret code.  
After each guess, the player receives feedback showing how many digits are correct and whether they are in the correct position.

---

## ✨ Features

- Random 3-digit secret code
- Supports leading zeros such as `042`
- Limited number of attempts
- Input validation
- Hint system
- Terminal-based gameplay
- Clean and simple C implementation

---

## 🎮 Game Rules

The secret code contains exactly 3 digits.

After each guess, the program gives two hints:

- **Right place**: correct digit in the correct position
- **Wrong place**: correct digit but in a different position

Example:

```text
Secret code: 123
Guess: 132

Hint: 1 right place, 2 wrong place
