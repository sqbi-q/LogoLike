# LogoLike

LogoLike is C project for educational purpose, inspired by Logo language.
This repository is heart of whole LogoLike. Code needs generated project folder to display canvas. 

---

If you are searching for graphical integrated canvas maker (that can initalize and edit canvas) for LogoLike, you may want to see [LogoLike-GUI](https://github.com/sqbi-q/LogoLike-GUI).

## What is Logo?

Logo is an educational programming language, designed in 1967 by Wally Feurzeig, Seymour Papert, and Cynthia Solomon. Logo is widely known for its use of turtle graphics, in which commands for movement and drawing produced line or vector graphics, either on screen or with a small robot termed a turtle.

## Installation

First, [clone](https://git-scm.com/book/en/v2/Git-Basics-Getting-a-Git-Repository) this project using [git-clone](https://git-scm.com/docs/git-clone):

```bash
git clone https://github.com/sqbi-q/LogoLike.git --recurse-submodules --remote-submodules
```
Using `--recurse-submodules` and `--remote-submodules` clones submodules [jsmn](https://github.com/zserge/jsmn) and [map](https://github.com/rxi/map) that are required by code.
Then when you already have project on disk, you need to compile it.

---

Right now, at this moment, there is only bash script compiling whole project using `gcc` (for Linux).


```bash
cd ./LogoLike/COMPILE/
./compile_gcc.sh
```
`compile_gcc.sh` outputs `LogoLike_output` - executable file
 

## Usage

Raw LogoLike needs initalized project folder to run.
Project folder and `setting.json` are files generated by [LogoLike-gui](https://github.com/sqbi-q/LogoLike-GUI) - graphical environment written in Java, allowing building canvas of LogoLike. 

This script should be executed by LogoLike-GUI.
```bash
./LogoLike_output <RELATIVE_PATH_TO_SETTING.JSON>
``` 
Besides running compiled LogoLike code, it is passing relative path to `setting.json` inside project folder.

## License
[GNU GPLv3](https://choosealicense.com/licenses/gpl-3.0/)