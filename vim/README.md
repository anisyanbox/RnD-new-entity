# VIM's tips

### Edit ~/vimrc
- Setup `~/.vimrc` file: https://gist.github.com/anisyanka/af16e6fb0515d2fe95c531f588f719c8 
- Generate tags `ctags -R --c++-kinds=+p --fields=+iaS --extra=+q /usr/include` for autocompletion

### Packages
See https://github.com/vim/vim/blob/bbf9f344afd08954163191ed678352fb554fc254/runtime/doc/repeat.txt#L528 as a 'howto' about VIM's packages.
VIM can load packages automatically from `~/.vim/pack/<any-name>/start` directory.
Also there is ability for optional plugins. They should be placed into `~/.vim/pack/<any-name>/opt` directory.
But in this case, if you want to use the plugin, you have to edit your `.vimrc` file: >
```
	:packadd! foobarpack
```
Also we can load plugins immediatly when vim starts up.
Just create `~/.vim/after/plugin/*.vim` file with commands.
For example file `fs-explorer.vim`. 
```sh
$ cat ~/.vim/after/plugin/nerdtree/fs-explorer.vim
NERDTree
```
It's a command to start the file system explorer.<CR>
But it will be better if we'll just map the command to some of Fxx buttons.<CR>
See the vimrc-file in the gist.

### Color schemes
To install a color scheme you need to download one `*.vim` file into `~/.vim/colors` directory.
Then just to edit vimrc file:
```sh
" add these lines
syntax on
colorscheme your/color/scheme/name
```

### Usefull packages
 - File system explorer: https://github.com/preservim/nerdtree
 - Autocompletion: https://github.com/ycm-core/YouCompleteMe
 - Themes: https://github.com/rafi/awesome-vim-colorschemes

