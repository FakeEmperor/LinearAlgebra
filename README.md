# LinearAlgebra
Linear Algebra is fun! :rooster:

## What is it?
It's an implementation of different applications of Linear Algebra in code.
There will be several different aspects of Linear Algebra in future: error-correction codes, cryptography, hashing. Right now there is only a collection of basic algorithms and objects of Linear Algeba to understand basic concepts of Computational Linear Algebra and theory behind it.
### Linear Algebra
There are several classes, either representing entity of Linear Algebra or used to support algorithms and applications.
#### Polynomial< Zp >
Represents a polynomial over field Z[x] with modulo Zp. Coefficients of a polynomial are taken from field Z.

#### GaloisFieldExtension< Zp, Degree >
Represents Galois Field with _Zp^Degree_ elements.
With this class you can factorize over elements of a field, find irreducible Polynomials, and, of course, build a finite field.

**Important note: Zp must be _prime_ number!** Or you cannot build multiplicative group inside a field using irreducible polynomial because of [Zero divisors](https://en.wikipedia.org/wiki/Zero_divisor) in rings with non-prime modulo. Read [Wikipedia](https://en.wikipedia.org/wiki/Finite_field#Non-prime_fields) for more.
#### PolynomialGenerator< Zp, Degree >
An interface for supporing classes, which will generate sequences of Polynomials of given degree over field Z[x] with modulo Zp.
##### ConsecutivePolynomialGenerator< Zp, Degree >
Simple implementaion of the interface. Use this to generate all Polynomials of given degree consecutively. It can tell, when all Polynomials are generated, but there may be quite a lot Polynomials to generate if _Degree_ is high.
##### RandomPolynomialGenerator< Zp, Degree >
Random Polynomial Generator generates, as name implies, random polynomial of given degree over field Z[x]. Polynomials can and will repeat on repeated usage. **Canot tell** when all possible Polynomials of given degree are generated.

#### Factorizer
Supporting class used to factorize number in multiplication series of prime numbers. Uses naive implementation.

#### Examples
You can see usage examples inside *example.cc*

## How to use
You can use this project as supporting material when studying Linear Algebra and as an illustration to how Linear Algebra can be applied in practical solutions. It is advised **against** the use in industrial applications, and should only be used in educational purposes.

## How to run
You need a compiler supporting C++11. No additional libraries required.
### Visual Studio
Requirements: Visual Studio 2015.
Open the solution inside */VisualStudio* folder.

