int main(void) {
  int a = 4;
  a += 4;
  int z = 9;

  const int b = 6;
  // b += 4; // fails to compile
  int const c = 7;
  // c += 4; // also fails to compile
  
  const int *ptr = &a;
  // *ptr += 1; // fails to compile
  ptr = &b;
  // tldr can change address but not the data pointed to by const {type} id;

  // same thing
  int const *ptr2 = &a;
  // *ptr += 1; // fails to compile
  ptr = &b;
  // tldr can change address but not the data pointed to by const {type} id;
  
  int *const ptr3 = &z;
  // ptr3 = &a; // fails to compile
  *ptr3 += 100;

}
