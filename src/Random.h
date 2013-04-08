/*** Function prototypes for R250 random number generator, by W. L. Maier  ***/
unsigned int r250();
unsigned int my_random (unsigned n);
double dr250();

/***
To: kpetersen@wsmr-simtel20.army.mil
Subject: r250 - portable random number generator
-------

Keith -

I have uploaded:

<msdos.c>
r250.zip          portable pseudorandom number generator

This is an implementation by W. L. Maier of the random number
generator described by S. Kirkpatrick and E. Stoll, Journal of
Computational Physics, 40, p. 517 (1981).  The generator uses 16
parallel copies of a linear shift register with period 2^250 - 1.  FAR
longer period than the usual linear congruent generator, and commonly
faster as well.  References are supplied.

I have made changes for portability: Initialization is optional because
the generator starts in a known, valid state.  A linear congruent
random number generator is supplied for initialization, so r250 should
generate the same sequence of numbers on any platform with 16 bit
integers.

- Jim Van Zandt (jrv@mbunix.mitre.org)
***/
