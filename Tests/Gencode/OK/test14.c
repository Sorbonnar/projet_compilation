// Expected output:
// x = 1
// y = 1048575
// add = 1048576
// sub = -1048574
// mul = 1048575
// div = 524287
//

void main() {
    int x = 0x1;
    int y = 0xFFFFF; // Overflow de ori

    int add = x + y;
    int sub = x - y;
    int mul = x * y;
    int div = y / (x + 1);

    print("x = ", x);
    print("\ny = ", y);

    print("\nadd = ", add);
    print("\nsub = ", sub);
    print("\nmul = ", mul);
    print("\ndiv = ", div);
}
