class GreatestCommonDivisor {
  int a = 10;
  int b = 20;
  //int y, j, k;
  int y = 9;
  bool n() {
    int x, y, z;
    bool f, t[8];
    x = a;
    y = b;
    y[8+3] = (m+7)*3;
    z = gcd(x, y);
    print z;
  }

// Función que calcula el máximo común divisor
  int gcd(int a, int b) {
    if (b == 0) { return(a); }
    else { return( gcd(b, a % b) ); }
  }
}
