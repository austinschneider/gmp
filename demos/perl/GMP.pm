# GMP perl module

# Copyright 2001, 2002, 2003 Free Software Foundation, Inc.
#
# This file is part of the GNU MP Library.
#
# The GNU MP Library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# The GNU MP Library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
# MA 02111-1307, USA.


# This code is designed to work with perl 5.005, so it and the sub-packages
# aren't as modern as they could be.

package GMP;

require Symbol;
require Exporter;
require DynaLoader;
@ISA = qw(Exporter DynaLoader);

@EXPORT = qw();
@EXPORT_OK = qw(version);
%EXPORT_TAGS = ('all' => [qw(
                             get_d get_d_2exp get_si get_str integer_p
                             printf sgn sprintf)],
		'constants' => [()]);
Exporter::export_ok_tags('all');

$VERSION = '1';
bootstrap GMP $VERSION;


# The format string is cut up into "%" specifiers so GMP types can be
# passed to GMP::sprintf_internal.  Any "*"s are interpolated before
# calling sprintf_internal, which saves worrying about variable
# argument lists there.
#
# Because sprintf_internal is only called after the conversion and
# operand have been checked there won't be any crashes from a bad
# format string.
#
sub sprintf {
  my $fmt = shift;
  my $out = '';
  my ($pre, $dummy, $pat, $rest);

  while (($pre, $dummy, $pat, $rest) = ($fmt =~ /^((%%|[^%])*)(%[- +#.*hlLqv\d]*[bcdfeEgGinopsuxX])(.*)$/s)) {

    $out .= $pre;

    my $pat2 = $pat;    # $pat with "*"s expanded
    my @params = ();    # arguments per "*"s
    while ($pat2 =~ /[*]/) {
      my $arg = shift;
      $pat2 =~ s/[*]/$arg/;
      push @params, $arg;
    }

    if (UNIVERSAL::isa($_[0],"GMP::Mpz")) {
      if ($pat2 !~ /[dioxX]$/) {
	die "GMP::sprintf: unsupported output format for mpz: $pat2\n";
      }
      $pat2 =~ s/(.)$/Z$1/;
      $out .= sprintf_internal ($pat2, shift);

    } elsif (UNIVERSAL::isa($_[0],"GMP::Mpq")) {
      if ($pat2 !~ /[dioxX]$/) {
	die "GMP::sprintf: unsupported output format for mpq: $pat2\n";
      }
      $pat2 =~ s/(.)$/Q$1/;
      $out .= sprintf_internal ($pat2, shift);

    } elsif (UNIVERSAL::isa($_[0],"GMP::Mpf")) {
      if ($pat2 !~ /[eEfgG]$/) {
	die "GMP::sprintf: unsupported output format for mpf: $pat2\n";
      }
      $pat2 =~ s/(.)$/F$1/;
      $out .= sprintf_internal ($pat2, shift);

    } elsif ($pat =~ /n$/) {
      # do it this way so h, l or V type modifiers are respected, and use a
      # dummy variable to avoid a warning about discarding the value
      my $dummy = sprintf "%s$pat", $out, $_[0];
      shift;

    } else {
      $out .= sprintf $pat, @params, shift;
    }

    $fmt = $rest;
  }
  $out .= $fmt;
  return $out;
}

sub printf {
  if (ref($_[0]) eq 'GLOB') {
    my $h = Symbol::qualify_to_ref(shift, caller);
    print $h GMP::sprintf(@_);
  } else {
    print STDOUT GMP::sprintf(@_);
  }
}

1;
__END__



=head1 NAME

GMP - Perl interface to the GNU Multiple Precision Arithmetic Library

=head1 SYNOPSIS

    use GMP;
    use GMP::Mpz;
    use GMP::Mpq;
    use GMP::Mpf;
    use GMP::Rand;

=head1 DESCRIPTION

This module provides access to GNU MP arbitrary precision integers,
rationals and floating point.

No functions are exported from these packages by default, but can be
selected in the usual way, or the tag :all for everything.

    use GMP::Mpz qw(gcd, lcm);   # just these functions
    use GMP::Mpq qw(:all);       # everything in mpq

=head2 GMP::Mpz

This class provides arbitrary precision integers.  A new mpz can be
constructed with C<mpz>.  The initial value can be an integer, float,
string, mpz, mpq or mpf.  Floats, mpq and mpf will be automatically
truncated to an integer.

    use GMP::Mpz qw(:all);
    my $a = mpz(123);
    my $b = mpz("0xFFFF");
    my $c = mpz(1.5);       # truncated

The following overloaded operators are available, and corresponding
assignment forms like C<+=>,

=over 4

=item

+ - * / % E<lt>E<lt> E<gt>E<gt> ** & | ^ ! E<lt> E<lt>= == != E<gt> E<gt>=
E<lt>=E<gt> abs not sqrt

=back

C</> and C<%> round towards zero (as per the C<tdiv> functions in GMP).

The following functions are available, behaving the same as the
corresponding GMP mpz functions,

=over 4

=item

bin, cdiv, cdiv_2exp, clrbit, combit, congruent_p, congruent_2exp_p,
divexact, divisible_p, divisible_2exp_p, even_p, fac, fdiv, fdiv_2exp, fib,
fib2, gcd, gcdext, hamdist, invert, jacobi, kronecker, lcm, lucnum, lucnum2,
mod, mpz_export, mpz_import, nextprime, odd_p, perfect_power_p,
perfect_square_p, popcount, powm, probab_prime_p, realloc, remove, root,
roote, scan0, scan1, setbit, sizeinbase, sqrtrem, tdiv, tdiv_2exp, tstbit

=back

C<cdiv>, C<fdiv> and C<tdiv> and their C<2exp> variants return a
quotient/remainder pair.  C<fib2> returns a pair F[n] and F[n-1], similarly
C<lucnum2>.  C<gcd> and C<lcm> accept a variable number of arguments (one or
more).  C<gcdext> returns a triplet of gcd and two cofactors, for example

    use GMP::Mpz qw(:all);
    $a = 7257;
    $b = 10701;
    ($g, $x, $y) = gcdext ($a, $b);
    print "gcd($a,$b) is $g, and $g == $a*$x + $b*$y\n";

C<mpz_import> and C<mpz_export> are so named to avoid the C<import> keyword.
Their parameters are as follows,

    $z = mpz_import ($order, $size, $endian, $nails, $string);
    $string = mpz_export ($order, $size, $endian, $nails, $z);

The order, size, endian and nails parameters are as per the corresponding C
functions.  The string input for C<mpz_import> is interpreted as byte data
and must be a multiple of size bytes.  C<mpz_export> conversely returns a
string of byte data, which will be a multiple of size bytes.

C<invert> returns the inverse, or undef if it doesn't exist.  C<remove>
returns a remainder/multiplicty pair.  C<root> returns the nth root, and
C<roote> returns a root/bool pair, the bool indicating whether the root is
exact.  C<sqrtrem> and C<rootrem> return a root/remainder pair.

C<clrbit>, C<combit> and C<setbit> expect a variable which they can modify,
it doesn't make sense to pass a literal constant.  Only the given variable
is modified, if other variables are referencing the same mpz object then a
new copy is made of it.  If the variable isn't an mpz it will be coerced to
one.  For instance,

    use GMP::Mpz qw(setbit);
    setbit (123, 0);  # wrong, don't pass a constant
    $a = mpz(6);
    $b = $a;
    setbit ($a, 0);   # $a becomes 7, $b stays at 6

C<scan0> and C<scan1> return ~0 if no 0 or 1 bit respectively is found.

=head2 GMP::Mpq

This class provides rationals with arbitrary precision numerators and
denominators.  A new mpq can be constructed with C<mpq>.  The initial value
can be an integer, float, string, mpz, mpq or mpf, or a pair of integers or
mpz's.  No precision is lost when converting a float or mpf, the exact value
is retained.

    use GMP::Mpq qw(:all);
    $a = mpq();              # zero
    $b = mpq(0.5);           # gives 1/2
    $b = mpq(14);            # integer 14
    $b = mpq(3,4);           # fraction 3/4
    $b = mpq("7/12");        # fraction 7/12
    $b = mpq("0xFF/0x100");  # fraction 255/256

When a fraction is given, it should be in the canonical form specified in
the GMP manual, which is denominator positive, no common factors, and zero
always represented as 0/1.  If not then C<canonicalize> can be called to put
it in that form.  For example,

    use GMP::Mpq qw(:all);
    $q = mpq(21,15);   # eek! common factor 3
    canonicalize($q);  # get rid of it

The following overloaded operators are available, and corresponding
assignment forms like C<+=>,

=over 4

=item

+ - * / E<lt>E<lt> E<gt>E<gt> ** ! E<lt> E<lt>= == != E<gt> E<gt>=
E<lt>=E<gt> abs not

=back

The following functions are available,

=over 4

=item

den, inv, num

=back

C<inv> calculates 1/q, as per the corresponding GMP function.  C<num> and
C<den> return an mpz copy of the numerator or denominator respectively.  In
the future C<num> and C<den> might give lvalues so the original mpq can be
modified through them, but this is not done currently.

=head2 GMP::Mpf

This class provides arbitrary precision floating point numbers.  The
mantissa is an arbitrary user-selected precision and the exponent is a fixed
size (one machine word).

A new mpf can be constructed with C<mpf>.  The initial value can be an
integer, float, string, mpz, mpq or mpf.  The second argument specifies the
desired precision in bits, or if omitted then the default precision is used.

    use GMP::Mpf qw(:all);
    $a = mpf();         # zero
    $b = mpf(-7.5);     # default precision
    $c = mpf(1.5, 500); # 500 bits precision
    $d = mpf("1.0000000000000001");

The following overloaded operators are available, with the corresponding
assignment forms like C<+=>,

=over 4

=item

+ - * / E<lt>E<lt> E<gt>E<gt> ** ! E<lt> E<lt>= == != E<gt> E<gt>=
E<lt>=E<gt> abs not sqrt

=back

The following functions are available, behaving the same as the
corresponding GMP mpf functions,

=over 4

=item

ceil, floor, get_default_prec, get_prec, mpf_eq, set_default_prec, set_prec,
trunc

=back

C<mpf_eq> is so named to avoid clashing with the perl C<eq> operator.

C<set_prec> expects a variable which it can modify, it doesn't make sense to
pass a literal constant.  Only the given variable is modified, if other
variables are referencing the same mpf object then a new copy is made of it.
If the variable isn't an mpf it will be coerced to one.

Results are the same precision as inputs, or if two mpf's are given to a
binary operator then the precision of the first is used.  For example,

    use GMP::Mpf qw(mpf);
    $a = mpf(2.0, 100);
    $b = mpf(2.0, 500);
    $c = $a + $b;         # gives 100 bits precision

Mpf to string conversion via "" or the usual string contexts uses C<$#> the
same as normal float to string conversions, or defaults to C<%.g> if C<$#>
is not defined.  C<%.g> means all significant digits in the selected
precision.

=head2 GMP class

The following functions are available in the GMP class,

=over 4

=item

fits_slong_p, get_d, get_d_2exp, get_si, get_str, integer_p, printf, sgn,
sprintf, version

=back

C<get_d_2exp> accepts any integer, string, float, mpz, mpq or mpf operands
and returns a float and an integer exponent,

    ($dbl, $exp) = get_d_2exp (mpf ("3.0"));
    # dbl is 0.75, exp is 2

C<get_str> takes an optional second argument which is the base, defaulting
to decimal.  A negative base means upper case, as per the C functions.  For
integer, integer string, mpz or mpq operands a string is returned.

    use GMP qw(:all);
    use GMP::Mpq qw(:all);
    print get_str(mpq(-5,8)),"\n";      # -5/8
    print get_str(255,16),"\n";         # ff

For float, float strings or mpf operands, C<get_str> accepts an optional
third parameter being how many digits to produce, defaulting to 0 which
means all digits.  (Only as many digits as can be accurately represented by
the float precision are ever produced though.)  A string/exponent pair is
returned, as per the C mpf_get_str function.  For example,

    use GMP qw(:all);
    use GMP::Mpf qw(:all);
    ($s, $e) = get_str(111.111111111, 10, 4);
    printf ".$se$e\n";                  # .1111e3
    ($s, $e) = get_str(1.625, 10);
    print "0.$s*10^$e\n";               # 0.1625*10^1
    ($s, $e) = get_str(mpf(2)**20, 16);
    printf ".%s@%x\n", $s, $e;          # .1@14

C<printf> and C<sprintf> allow formatted output of GMP types.  mpz and mpq
values can be used with integer conversions (d, o, x, X) and mpf with float
conversions (f, e, E, g, G).  All the standard perl printf features are
available too.  For example,

    use GMP::Mpz qw(mpz);
    use GMP::Mpf qw(mpf);
    GMP::printf ("%d %d %s", 123, mpz(2)**128, 'foo');
    GMP::printf STDERR "%.40f", mpf(1.234);

In perl 5.6.1 it doesn't seem to work to export C<printf>, the plain builtin
C<printf> is reached unless calls are C<&printf()> style.  Explicit use of
C<GMP::printf> is suggested.  C<sprintf> doesn't suffer this problem.

    use GMP qw(sprintf);
    use GMP::Mpq qw(mpq);
    $s = sprintf "%x", mpq(15,16);

C<version> is not exported by default or by tag :all, calling it as
C<GMP::version()> is recommended.  It returns the GMP library version
string, which is not to be confused with the module version number.

The other GMP module functions behave as per the corresponding GMP routines,
and accept any integer, string, float, mpz, mpq or mpf.  For example,

    use GMP qw(:all);
    use GMP::Mpz qw(mpz);
    $z = mpz(123);
    print sgn($z);    # gives 1

Because each of GMP::Mpz, GMP::Mpq and GMP::Mpf is a sub-class of GMP,
C<-E<gt>> style calls work too.

    use GMP qw(:all);
    use GMP::Mpq qw(mpf);
    $q = mpq(-5,7);
    if ($q->integer_p())   # false
      ...

=head2 GMP::Rand

This class provides objects holding an algorithm and state for random number
generation.  C<randstate> creates a new object, for example,

    use GMP::Rand qw(randstate);
    $r = randstate();
    $r = randstate('lc_2exp_size', 64);
    $r = randstate('lc_2exp', 43840821, 1, 32);
    $r = randstate('mt');
    $r = randstate($another_r);

With no parameters this corresponds to the C function
C<gmp_randinit_default>, and is a compromise between speed and randomness.
'lc_2exp_size' corresponds to C<gmp_randinit_lc_2exp_size>, 'lc_2exp'
corresponds to C<gmp_randinit_lc_2exp>, and 'mt' corresponds to
C<gmp_randinit_mt>.  Or when passed another randstate object, a copy of that
object is made.

'lc_2exp_size' can fail if the requested size is bigger than the internal
table provides for, in which case undef is returned.  The maximum size
currently supported is 128.  The other forms always succeed.

A randstate can be seeded with an integer or mpz, using the C<seed> method.
/dev/random might be a good source of randomness, or time() or
Time::HiRes::time() might be adequate, depending on the application.

    $r->seed(time()));

Random numbers can be generated with the following functions,

=over 4

=item

mpf_urandomb, mpz_rrandomb, mpz_urandomb, mpz_urandomm,
gmp_urandomb_ui, gmp_urandomm_ui

=back

Each constructs a new mpz or mpf and with a distribution per the
corresponding GMP function.  For example,

    use GMP::Rand (:all);
    $r = randstate();
    $a = mpz_urandomb($r,256);         # uniform mpz, 256 bits
    $b = mpz_urandomm($r,mpz(3)**100); # uniform mpz, 0 to 3**100-1
    $c = mpz_rrandomb($r,1024);        # special mpz, 1024 bits
    $f = mpf_urandomb($r,128);         # uniform mpf, 128 bits, 0<=$f<1
    $f = gmp_urandomm_ui($r,56);       # uniform int, 0 to 55

=head2 Coercion

Arguments to operators and functions are converted as necessary to the
appropriate type.  For instance C<**> requires an unsigned integer exponent,
and an mpq argument will be converted, so long as it's an integer in the
apropriate range.

    use GMP::Mpz (mpz);
    use GMP::Mpq (mpq);
    $p = mpz(3) ** mpq(45);   # allowed, 45 is an integer

It's an error if a conversion to an integer or mpz would cause any
truncation.  For example,

    use GMP::Mpz (mpz);
    $p = mpz(3) + 1.25;       # not allowed
    $p = mpz(3) + mpz(1.25);  # allowed, explicit truncation

Comparisons, however, accept any combination of operands and are always done
exactly.  For example,

    use GMP::Mpz (mpz);
    print mpz(3) < 3.1;       # true

Variables used on the left of an assignment operator like C<+=> are subject
to coercion too.  An integer, float or string will change type when an mpz,
mpq or mpf is applied to it.  For example,

    use GMP::Mpz (mpz);
    $a = 1;
    $a += mpz(1234);   # $a becomes an mpz

=head2 Overloading

The rule for binary operators in the C<overload> mechanism is that if both
operands are class objects then the method from the first is used.  This
determines the result type when mixing GMP classes.  For example,

    use GMP::Mpz (mpz);
    use GMP::Mpq (mpq);
    use GMP::Mpf (mpf);
    $z = mpz(123);
    $q = mpq(3,2);
    $f = mpf(1.375)
    print $q+$f;     # gives an mpq
    print $f+$z;     # gives an mpf
    print $z+$f;     # not allowed, would lose precision

=head2 Constants

A special tag C<:constants> is recognised in the module exports list.  It
doesn't select any functions, but indicates that perl constants should be
GMP objects.  This can only be used on one of GMP::Mpz, GMP::Mpq or GMP::Mpf
at any one time, since they apply different rules.

GMP::Mpz will treat constants as mpz's if they're integers, or ordinary
floats if not.  For example,

    use GMP::Mpz qw(:constants);
    print 764861287634126387126378128,"\n";   # an mpz
    print 1.25,"\n";                          # a float

GMP::Mpq is similar, treating integers as mpq's and leaving floats to the
normal perl handling.  Something like 3/4 is read as two integer mpq's and a
division, but that's fine since it gives the intended fraction.

    use GMP::Mpq qw(:constants);
    print 3/4,"\n";    # an mpq
    print 1.25,"\n";   # a float

GMP::Mpf will treat all constants as mpf's using the default precision.
BEGIN blocks can be used to set that precision while the code is parsed.
For example,

    use GMP::Mpf qw(:constants);
    BEGIN { GMP::Mpf::set_default_prec(256); }
    print 1/3;
    BEGIN { GMP::Mpf::set_default_prec(64); }
    print 5/7;

A similar special tag :noconstants is recognised to turn off the constants
feature.  For example,

    use GMP::Mpz qw(:constants);
    print 438249738748174928193,"\n";   # an mpz
    use GMP::Mpz qw(:noconstants);
    print 438249738748174928193,"\n";   # now a float

All three 'integer', 'binary' and 'float' constant methods are captured.
'float' is captured even for GMP::Mpz and GMP::Mpq since perl by default
treats integer strings as floats if they don't fit a plain integer.

=head1 SEE ALSO

GMP manual, L<perl>, L<overload>.

=head1 BUGS

In perl 5.005_03 on i386 FreeBSD, the overloaded constants sometimes provoke
seg faults.  Don't know if that's a perl bug or a GMP module bug, though it
does seem to go bad before reaching anything in GMP.xs.

There's no way to specify an arbitrary base when converting a string to an
mpz (or mpq or mpf), only hex or octal with 0x or 0 (for mpz and mpq, but
not for mpf).

These modules are not reentrant or thread safe, due to the implementation of
the XSUBs.

Returning a new object from the various functions is convenient, but
assignment versions could avoid creating new objects.  Perhaps they could be
named after the C language functions, eg. mpq_inv($q,$q);

It'd be good if C<num> and C<den> gave lvalues so the underlying mpq could
be manipulated.

C<printf> could usefully accept %b for mpz, mpq and mpf, and perhaps %x for
mpf too.

C<get_str> returning different style values for integer versus float is a
bit unfortunate.  With mpz, mpq and mpf objects there's no doubt what it
will do, but on a plain scalar its action depends on whether the scalar was
promoted to a float at any stage, and then on the GMP module rules about
using the integer or float part.

There's no interface to mpfr.

=head1 INTERNALS

In usual perl object style, an mpz is a reference to an object blessed into
class C<GMP::Mpz>.  The object holds a pointer to the C language C<mpz_t>
structure.  Similarly for mpq, mpf and randstate.

A free list of mpz and mpq values is kept to avoid repeated initializing and
clearing when objects are created and destroyed.  This aims to help speed,
but it's not clear whether it's really needed.

mpf doesn't use a free list because the precision of new objects can be
different each time.

No interface to C<mpf_set_prec_raw> is provided.  It wouldn't be very useful
since there's no way to make an operation store its result in a particular
object.  The plain C<set_prec> is useful though, for truncating to a lower
precision, or as a sort of directive that subsequent calculations involving
that variable should use a higher precision.

The overheads of perl dynamic typing (operator dispatch, operand type
checking or coercion) will mean this interface is slower than using C
directly.

Some assertion checking is available as a compile-time option.

=cut

# Local variables:
# perl-indent-level: 2
# fill-column: 76
# End:
