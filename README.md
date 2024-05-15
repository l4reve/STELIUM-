# 🚀💣🎮

> Welcome to Minesweeper ;3

<< x86 MICROSOFT VISUAL STUDIO DEBUG >>

![Screenshot 2024-05-14 212558](https://github.com/l4reve/txtms/assets/143638421/9e565d2c-4cdd-4572-bcc3-de09b2bfe409)

## Rules

1. **Objective**: Uncover all non-mine squares without triggering any mines.

![Screenshot 2024-05-14 224426](https://github.com/l4reve/txtms/assets/143638421/9b7cc9b9-f8fe-4a8d-9bbb-0ce76f9deebe)

![Screenshot 2024-05-14 224005](https://github.com/l4reve/txtms/assets/143638421/06b2c6f3-b427-4f8b-9d90-86198661970e)

2. **Gameplay**: Clone all of files in this repo

- Make an empty project in visual studio
- Goto folder which has project file(.vcxproj) and then paste all of files we cloned
- Add source files and header files
- In visual studio, go to Project -> properties -> Linker -> Input -> Additional dependencies
- Add SDL2_ttf.lib , SDL2_image.lib , SDL2main.lib , SDL2_mixer.lib , SDL2.lib
- Go to Project -> properties -> Configuration Properties -> Include Directories
- Add SDL2_mixer-devel-your version-VC/include , SDL2_ttf-devel-your version-VC/include , SDL2_image-your version/include , SDL2-devel-your version-VC/include
- Go to Project -> properties -> Configuration Properties -> Library Directories
- Add SDL2_mixer-devel-your version-VC/lib/x86, SDL2_mixer-devel-your version-VC/lib/x86, SDL2_image-your version/lib/x86, SDL2-devel-your version-VC/lib/x86


3. **Winning**: Clear all non-mine squares to triumph!

![Screenshot 2024-05-14 224122](https://github.com/l4reve/txtms/assets/143638421/c2470600-a1a6-4e96-9c75-46bee08b6345)

## Starting

- **Installation**: Clone this repo, instal SDL2 (make sure to link proper x86 include/lib), compile, and run.
- **Dependencies**: C++, MS Visual Studio, SDL libs

🎮✨

