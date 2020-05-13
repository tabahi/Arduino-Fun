
String s = "";


void setup()
{
  Serial.begin(9600);
  s.reserve(10);
  //int sum = 15;
  //int n = dimFinder(sum);
  //MagicSquare(3);
}


void loop()
{
  while (Serial.available())
  {
    char c = Serial.read();
    s += c;
    if (s.indexOf('N') >= 0)
    {
      s = s.substring(s.indexOf('N'));
    }
    if (s.indexOf(';') > 0)
    {
      s = s.substring(0, s.indexOf(';'));
      s = s.substring(2);
      Serial.print("Input:\t");
      Serial.println(s);
      int input = s.toInt();
      if (input % 2)
      {
        MagicSquare(input);
      }
      else
      {
        Serial.println("ERROR: EVEN NUM");
      }
    }
  }
  delay(500);
  Serial.println("!");


}

void MagicSquare(int n)
{
  int M[n][n];

  // Clearing all locations
  for (int j = 0 ; j < n ; j++) {
    for (int k = 0 ; k < n ; k++) {
      M[j][k] = 0;
    }
  }

  //Finding the center location
  int i = n / 2;
  int j = n - 1;

  for (int num = 1; num <= n * n; )
  {
    if (i == -1 && j == n) {
      j = n - 2;
      i = 0;
    }
    else {
      if (j == n)
        j = 0;
      if (i < 0)
        i = n - 1;
    }

    if (M[i][j]) {
      j -= 2;
      i++;
      continue;
    }
    else
      M[i][j] = num++;

    j++;
    i--;
  }

  //Serial.println("The Magic Square for n= " + String(n) + "\n");

  // print magic square
  Serial.print("@,");
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      Serial.print(M[i][j]);
      Serial.print(",");
    }
    Serial.print("#,");
  }
  int ssum = 0;
  for (j = 0; j < n; j++)
  {
    ssum += M[0][j];
  }
  
  Serial.print("&,");
  Serial.print(ssum);
  Serial.print(",&,");
  Serial.print(n * n);
  Serial.print(",");

  Serial.println(";");
}

int dimFinder(int sum) {
  int n_sum = 0;
  int n = 3;

  while (sum > n_sum) {
    n_sum = (n * (pow(n, 2) + 1)) / 2;

    if (sum == n_sum)
      return n;
    else
      n++;
  }
  return 0;
}

