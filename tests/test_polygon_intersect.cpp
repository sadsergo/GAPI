
void
test_polygon_intersect()
{
    std::vector<Vec<4, float>> P1, P2;
    float coordsP1[10] = {16, -3, 28, 16, 16, 32, 0, 22, 3, 10}; 
    // float coordsP2[20] = {0, 16, 5, 8, 13, 0, 19, 2, 24, 10, 24, 26, 19, 29, 13, 32, 7, 32, 3, 29};
    float p[6] = {0, 5, 5, 5, 10, 10};

    for (int i = 0; i < 5; ++i) {
      float c[4];

      c[0] = coordsP1[2 * i];
      c[1] = coordsP1[2 * i + 1];
      c[2] = 0;
      c[3] = 0;

      Vec<4, float> a(c);

      P1.push_back(a);
    }

    for (int i = 0; i < 3; ++i) {
      float c[4];

      c[0] = p[2 * i];
      c[1] = p[2 * i + 1];
      c[2] = 0;
      c[3] = 0;

      Vec<4, float> a(c);

      P2.push_back(a);
    }

    std::vector<Vec<4, float>> ans = intersect(P1, P2);

    for (auto el : ans) {
      el.show();
    } 
}