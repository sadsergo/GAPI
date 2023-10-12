#include "VBHB.h"
#include <algorithm>

int
orient(Vec<4, float> triangle[3])
{
  float ori = (triangle[1][0] * triangle[2][1] - triangle[1][1] * triangle[2][0]) - 
                (triangle[0][0] * triangle[2][1] - triangle[0][1] * triangle[2][0]) + 
                (triangle[0][0] * triangle[1][1] - triangle[0][1] * triangle[1][0]);
  
  return (ori < 0) ? -1 : (ori > 0) ? 1 : 0;
}

//! page 144
int
areaSign(const Vec<4, float>& a, const Vec<4, float>& b, const Vec<4, float>& c)
{
  double area2 = (b[X] - a[X]) * (double)(c[Y] - a[Y]) - 
                 (c[X] - a[X]) * (double)(b[Y] - a[Y]);

  if (area2 > 0.5) {
    return 1;
  }
  else if (area2 < -0.5) {
    return -1;
  }

  return 0;
}

//! Page 225
void 
Assigndi(Vec<4, float>& p, const Vec<4, float>& a)
{
  for (int i = 0; i < 4; ++i) {
    p[i] = a[i];
  }
}

//! Page 225
bool 
Between(const Vec<4, float>& a, const Vec<4, float>& b, const Vec<4, float>& c)
{
  if (!Collinear(a, b, c)) {
    return false;
  }

  if (a[X] != b[X]) {
    return ((a[X] <= c[X]) && (c[X] <= b[X])) || 
           ((a[X] >= c[X]) && (c[X] >= b[X]));
  }

  return ((a[Y] <= c[Y]) && (c[Y] <= b[Y])) || 
         ((a[Y] >= c[Y]) && (c[Y] >= b[Y]));
}

//! Page 127
bool 
Collinear(const Vec<4, float>& a, const Vec<4, float>& b, const Vec<4, float>& c)
{
  return ((c[Z] - a[Z]) * (b[Y] - a[Y]) - 
          (b[Z] - a[Z]) * (c[Y] - a[Y])) == 0 &&
         ((b[Z] - a[Z]) * (c[X] - a[X]) -
          (b[X] - a[X]) * (c[Z] - a[Z])) == 0 &&
         ((b[X] - a[X]) * (c[Y] - a[Y]) -
          (b[Y] - a[Y]) * (c[X] - a[X])) == 0;
}

//! Page 225
char 
ParallelInt(const Vec<4, float>& a, const Vec<4, float>& b, const Vec<4, float>& c, const Vec<4, float>& d, Vec<4, float>& p)
{
  if (!Collinear(a, b, c)) {
    return '0';
  }

  if (Between(a, b, c)) {
    Assigndi(p, c);
    return 'e';
  }

  if (Between(a, b, d)) {
    Assigndi(p, d);
    return 'e';
  }

  if (Between(c, d, a)) {
    Assigndi(p, a);
    return 'e';
  }

  if (Between(c, d, b)) {
    Assigndi(p, b);
    return 'e';
  }

  return '0';
}

//! Page 224
char 
SegSegInt(const Vec<4, float>& a, const Vec<4, float>& b, const Vec<4, float>& c, const Vec<4, float>& d, Vec<4, float>& p )
{
  double s = 0, t = 0; 
  double num = 0, denom = 0;
  char code = '?';

  /* The two parametersof theparametric eqns. */ /* Numerator and denominator of equations. */ /* Return char characterizing intersection. */
  denom = a[X] * (double)(d[Y] - c[Y]) +
          b[X] * (double)(c[Y] - d[Y]) + 
          d[X] * (double)(b[Y] - a[Y]) +
          c[X] * (double)(a[Y] - b[Y]);
  /* If denom is zero, then segments are parallel: handle separately. */ if (denom==0.0)
  
  if (denom == 0.0) {
    return ParallelInt(a, b, c, d, p);
  }

  num = a[X] * (double)(d[Y] - c[Y]) +
        c[X] * (double)(a[Y] - d[Y]) +
        d[X] * (double)(c[Y] - a[Y]);

  if (num == 0.0 || num == denom) {
    code = 'v';
  }

  s = num / denom;

  num = -(a[X] * (double)(c[Y] - b[Y]) +
          b[X] * (double)(a[Y] - c[Y]) +
          c[X] * (double)(b[Y] - a[Y]));

  if (num == 0.0 || num == denom) {
    code = 'v';
  }

  t = num / denom;

  if ((0.0 < s) && (s < 1.0) && (0.0 < t) && (t < 1.0)) {
    code = '1';
  }
  else if ((0.0 > s) || (s > 1.0) || (0.0 > t) || (t > 1.0)) {
    code = '0';
  }

  p[X] = a[X] + s * (b[X] - a[X]);
  p[Y] = a[Y] + s * (b[Y] - a[Y]);

  return code;
}

int 
InOut(const Vec<4, float>& tp, const int& inflag, const int& aHB, const int& bHA)
{
  // LineTo_d(p);
  tp.show();

  if (aHB > 0) {
    return Pin;
  }
  else if (bHA > 0) {
    return Qin;
  }

  return inflag;
}

int 
Advance(int a, int *aa, int n, bool inside, const Vec<4, float>& v)
{
  if (inside) {
    // LineTo_i(v);
    v.show();
  }

  (*aa)++;

  return (a + 1) % n;
}

std::vector<Vec<4, float>> 
intersect(const std::vector<Vec<4, float>>& P, const std::vector<Vec<4, float>>& Q)
{
  //! 257 page
  std::vector<Vec<4, float>> res;
  int a = 0, b = 0, a1 = 0, b1 = 0;
  Vec<4, float> A, B;
  int cross = 0, bHA = 0, aHB = 0;
  //* Origin = {0, 0, 0, 0}
  Vec<4, float> Origin;
  Vec<4, float> p, q;
  int inflag = Unknown, aa = 0, ba = 0;
  bool FirstPoint = true;
  Vec<4, float> p0;
  int code = 0;
  int64_t n = P.size(), m = Q.size();

  do {
    a1 = (a + n - 1) % n;
    b1 = (b + m - 1) % m;

    A = P[a] - P[a1];
    B = Q[b] - Q[b1];

    cross = areaSign(Origin, A, B);
    aHB = areaSign(Q[b1], Q[b], P[a]);
    bHA = areaSign(P[a1], P[a], Q[b]);

    code = SegSegInt(P[a1], P[a], Q[b1], Q[b], p);

    if (code == '1' || code == 'v') {
      if (inflag == Unknown && FirstPoint) {
        aa = ba = 0;
        FirstPoint = false;
        p0[X] = p[X];
        p0[Y] = p[Y];

        // MoveTo_d(p0);
        p0.show();
      }

      inflag = InOut(p, inflag, aHB, bHA);
    }

    if ((code == 'e') && dot(A, B) < 0) {
      // PrintSharedSeg(p, q);
      // p.show();
      exit(EXIT_SUCCESS);
    }

    if ((cross == 0) && (aHB < 0) && (bHA < 0)) {
      std::cout << "P and Q are disjoint" << std::endl;
      exit(EXIT_SUCCESS);
    }
    else if ((cross == 0) && (aHB == 0) && (bHA == 0)) {
      if (inflag == Pin) {
        b = Advance(b, &ba, m, inflag == Qin, Q[b]);
      }
      else {
        a = Advance(a, &aa, n, inflag == Pin, P[a]);
      }
    }
    else if (cross >= 0) {
      if (bHA > 0) {
        a = Advance(a, &aa, n, inflag == Pin, P[a]);
      }
      else {
        b = Advance(b, &ba, m, inflag == Qin, Q[b]);
      }
    }
    else {
      if (aHB > 0) {
        b = Advance(b, &ba, m, inflag == Qin, Q[b]);
      }
      else {
        a = Advance(a, &aa, n, inflag == Pin, P[a]);
      }
    }
  } while (((aa < n) || (ba < m)) && (aa < 2 * n) && (ba < 2 * m));

  if (!FirstPoint) {
    // LineTo(p0);
    p0.show();
  }

  if (inflag == Unknown) {
    //  Do not cross
  }

  return res;
}