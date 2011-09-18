# split

## What is split?
Ever hosted a party with friends and wanted to split the bill equally? This tool makes easy, generating the smallest possible amount of transactions.


## Get split
### Getting the source
If you are on Windows and unable to compile this yourself, download the precompiled [binary](http://time-t-able.psoke.de/).

But if you are able to, I recommend building it from source. To get the source you can use git (which I recommend):

	$ git clone git://github.com/Ps0ke/split.git
	
Or you can get the source from github's [tarball](https://github.com/Ps0ke/split/tarball/master) or [zip archive](https://github.com/Ps0ke/split/zipball/master). For online source browsing, use the the [github repository](https://github.com/Ps0ke/split)).

### Building & installation
To build and install on UNIX simply type (you need to authenticate you, this installation installs to `/usr/bin`, I provide a `make clean`, too!):

	$ cd split
	$ make
	$ sudo make install

On other operating systems use any standard C compiler. Build the `split` binary from the `main.c`.


## Usage
	$ split [-?] {[name] [money contributed]}...

`-?` shows the help. The amount of money contributed as a float value with ether comma or period as separator. You need to specify at least three pairs of data. The max length of a name can be set in the constants, standard is 10 characters. The max amount of money can be set in the constants, standard is `999.99`.

### Example
	$ split foo 2.30 bar 4.20 Ps0ke 5.00

### Output
	
	  2.30 foo
	  4.20 bar
	  5.00 Ps0ke

	Total:    11.50
	Average:   3.83

	Result:
	foo > 1.17 > Ps0ke
	foo > 0.37 > bar


## How does it work?
The data gets read and structured in a list. From that list I calculate the average and split the list into `give` and `get` lists - the ones that have to give money and the ones that will get money. People that already match the average get ignored automatically. In those two lists the difference from the average gets calculated and saved in the list. Then I sort the lists descending so the ones that get most are on top as well as the ones that have to give most. Then I take every `getter`, cycle through the `givers` and subtract as long as one of the both gets zero, save the transaction and proceed to the next `giver`/`getter`. After the last `getter` deserves an amount of `0.00` I finally print the output.


## License
Published under [cc-by-nc-sa](http://creativecommons.org/licenses/by-nc-sa/3.0/) (c) 2011 by Philipp Stephan, [ps0ke.de](http://ps0ke.de)


## Credits
Inspired by `Split` by [Felix Bruns](http://split.felixbruns.de/).
