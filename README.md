### Disclaimer: This project is not maintained by The Community Updates, 2.0, or AM2RLauncher developers. It is mine alone, do not reach out to them for support.
# AM2R-MinusLauncher
This is a replacement for the Linux version of the [AM2RLauncher](https://github.com/AM2R-Community-Developers/AM2RLauncher), which had its Flathub release [automatically de-listed](https://github.com/AM2R-Community-Developers/AM2RLauncher/issues/113#issuecomment-5681407714), and its binary release has been broken for as long as I've been active.
### Dependencies
NOTE: The provided names are based on the package name in Arch Linux's pacman repository. 
- `git` (used for obtaining the patch data)
- `xdelta3` (used for applying AM2R mod patches)
- `unzip` (used for extracting zip archives)
- `coreutils` (if you made it this long on Linux without coreutils, you are not in need of this program)
- AM2R 1.1
### Usage
You can find the program [here](https://github.com/rm-steele/AM2R-MinusLauncher/releases/latest). Unzip it wherever you prefer and can write into.
The launcher is a command-line interactive program. It takes no arguments, just execute it (run `./AM2R-MinusLauncher` in your favorite terminal) and follow its prompts.
You will need to give it the path to an AM2R 1.1 zip and any mod zip you wish to install. Any GUI terminal worth using will have a copy and paste function that you can use, or you can type it manually.
### Why not update the old launcher?
1. This program is brought to you by the `ANTI WEBAPP BRIGADE`. There will never be a web browser integrated into or used to display content in this launcher. Consequently, you can find the AM2R changelog [here](https://am2r-community-developers.github.io/DistributionCenter/changelog.html) and the AM2RLauncher news [here](https://am2r-community-developers.github.io/DistributionCenter/news.html), although I wouldn't bet on them changing any time soon.
2. I do not know the AM2RLauncher's codebase. I could learn it, and indeed I learned enough to document how it worked and write a compatible program. But I do not wish to learn How Eto and GTK are set up, try and update what's needed, and fix the bugs that come with updating its version.
   
   Furthermore, I'd be stuck contributing code to a team who self-admittedly will be quite busy and has a misplaced laundry list of things to do before my fixes would be likely to see release.
3. I enjoy writing code and doing new things. Despite reading a (1988) C Programming Guide last year, I had never made anything substantial in the language. Now I have, and I know what I do and don't like about the language.
### Notes
- This program is not intended to replace the launcher, and it will never be half as capable
- Despite my above ramblings, I do not wish negatively upon the launcher and the team behind it. Only towards WebKit, even when used as aptly as in the original launcher
- No integrity checking is done to the files provided. AM2R 1.1 is not verified to be unmodified, and mods are not verified to be for the right OS
### Compiling
See [compile.sh](/compile.sh), and translate to suit your favorite C compiler. To my knowledge, I have thus far only used headers from the GNU C Library.
### Contributing
No LLMs. Otherwise, feel free to open a pull request.
