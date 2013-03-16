# rdd - random data dump

rdd is a high-speed, cryptographically safe random data generator
using the reference implementation of the eSTREAM Profile 1 SOSEMANUK
stream cipher.

It is designed to fill crypto devices with random data.

This implementation passed dieharder 3.31.1 "-a" and
PractRand/RNG_test 0.90 (tested up to 512 GB).

## Usage

	rdd [-i SOURCE] [-r REKEYMB] [-v]
	   -i SOURCE     read key and IVs from SOURCE (/dev/urandom)
	   -r REKEYMB    reload the IV every REKEYMB megabytes of output (4)
           -v            print a . to stderr on every rekey

Random binary data is outputted to stdout.  Better don't put a
terminal there.
rdd reads 32 bytes for the initial key and 16 bytes for every rekeying
from the random source.  Setting REKEYMB to -1 never rekeys.

## Copyright

### rdd.c:

To the extent possible under law, Christian Neukirchen has waived
all copyright and related or neighboring rights to this work.

http://creativecommons.org/publicdomain/zero/1.0/

### sosemanuk.[ch]:

This program includes code taken from
http://www.ecrypt.eu.org/stream/p3ciphers/sosemanuk/sosemanuk_p3source.zip
as of 13mar2013.  An error message which can never occur in rdd usage
has been #ifdef'ed out.

(c) 2005 X-CRYPT project. This software is provided 'as-is', without
any express or implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to no restriction.
