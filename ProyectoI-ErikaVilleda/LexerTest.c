class GreatestCommonDivisor {
  int a = 10;
  int b = 20;
  void main() {
    int x, y, z;
    x = a;
    y = b;
    z = gcd(x, y);
    print z;
  }

// Función que calcula el máximo común divisor
  int gcd(int a, int b) {
    if (b == 0) { return(a); }
    else { return( gcd(b, a % b) ); }
  }
}
