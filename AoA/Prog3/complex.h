
/*
                ***** COMPLEX.H *****

Complex number class, to help illustrate fundamentals of C++ classes
and OOP approach to problem solving. Includes examples of constructors,
operator functions, and friend functions.

Author: John M. Weiss, Ph.D.
Class:  CSC 371
Date:   09/09/99

Modifications:
    Multiplication and absolute value added Fall 2002 by Ed Corwin
*/

#include <iostream>
using namespace std;

class complex
{
  private:
    // private data members
    float real, imag;

  public:
    // constructor (note use of initialization list)
    complex (float r = 0.0, float i = 0.0) : real(r), imag(i) {}

    // member functions (const member functions do not modify class data)
    float realpart() const { return real; }                     // real part
    float imagpart() const { return imag; }                     // imaginary part
    float abs() const { return sqrt(real * real + imag * imag); }     // absolute value 

    complex conj() const { return complex(real, -imag); }       // complex conjugate


    // friend operator functions
    friend complex operator+ (const complex&, const complex&);  // arithmetic ops
    friend complex operator- (const complex&, const complex&);
    friend complex operator* (const complex&, const complex&); 
    friend complex operator/ (const complex&, const complex&); 

    friend int operator== (const complex&, const complex&);     // relational ops
    friend int operator!= (const complex&, const complex&);

    friend istream& operator>> (istream&, complex&);            // input
    friend ostream& operator<< (ostream&, const complex&);      // output
};

// complex arithmetic operators, for complex addition and subtraction, multiplication

inline complex operator+ (const complex& z1, const complex& z2)
{
    return complex(z1.real + z2.real, z1.imag + z2.imag);
}

inline complex operator- (const complex& z1, const complex& z2)
{
    return complex(z1.real - z2.real, z1.imag - z2.imag);
}

inline complex operator* (const complex& z1, const complex& z2)
{
    return complex(z1.real * z2.real - z1.imag * z2.imag, 
                   z1.real * z2.imag + z1.imag * z2.real);
}

inline complex operator/ (const complex& z1, const complex& z2)
  {
  float   bottom;
  complex z2bar;
  complex top;

  bottom = z2.abs() * z2.abs();
  if (fabs(bottom) < 1.0e-20)
    {
    cerr << "attempt to do complex divide by zero" << endl;
    return complex(0, 0);
    }

  z2bar = z2.conj();
  top = z1 * z2bar;

  return complex(top.real / bottom, top.imag / bottom);
  }

// complex relational operators, to test for complex equality and inequality

inline int operator== (const complex& z1, const complex& z2)
{
    return z1.real == z2.real && z1.imag == z2.imag;
}

inline int operator!= (const complex& z1, const complex& z2)
{
    return z1.real != z2.real || z1.imag != z2.imag;
}

// complex I/O operators

// complex input (must be a friend function)
istream& operator>> (istream& in, complex& z)
{
    in >> z.real >> z.imag;
    return in;
}

// complex output (does not really need to be a friend function)
ostream& operator<< (ostream& out, const complex& z)
{
    out << '(' << z.real << ',' << z.imag << ')';
    return out;
}

