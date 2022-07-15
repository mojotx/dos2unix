# dos2unix

## Summary

Convert MS-DOS text files, with CRLF line endings, into UNIX format, with LF line endings.

## Details

The MS-DOS operating system, and the Microsoft Windows family of operating systems, has
the standard of ending each line with a carriage return character, ASCII 0x0d, and line
feed character, ASCII 0x0a.

However, UNIX-like operating systems, such as GNU/Linux and the various flavors of BSD,
end each line with a line feed character, ASCII 0x0a.

This utility takes one or more filenames as an argument, and converts the file to UNIX
format, and overwrites the original file.

It works by reading the input file, one byte at a time, and writing each byte to a
temporary file if the byte is NOT 0x0d, which is rendered in text as ^M. Once it's done,
it renames the temp file to the original file name, clobbering the file.

## Disclaimer

I wrote this for my own use. It should work well on macOS and Linux, but I did not try
to anticipate all use cases, or add all features. I wrote this in C for performance reasons,
and I am aware that there are other solutions, like the original
[dos2unix](https://waterlan.home.xs4all.nl/dos2unix.html) version, as well as alternative
solutions, like using `tr -d \015 < DOS-file > UNIX-file`, or `sed 's/^M$//' filename`.

## License

Licensed under the open source [MIT License](https://opensource.org/licenses/MIT).
