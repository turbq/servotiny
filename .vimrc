"Настройка табов 
set tabstop=4 
set softtabstop=4 
set shiftwidth=4 
set noexpandtab 
"Держать свои строки в пределах 110 символов 
set colorcolumn=110 
highlight ColorColumn ctermbg=darkgray

"По умолчанию, Vim предпологает, что все .h файлы это C++. Однако, часть моих проектов написаны на чистом C. Поэтому я хочу, чтобы тип файла был C. Кроме того, в моих проектах используется doxygen, так что я не против включить очень клевую подсветку doxygen в vim.
augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END
"Установка переменной path
let &path.="src/include,/usr/include/AL,/usr/avr/include,"

