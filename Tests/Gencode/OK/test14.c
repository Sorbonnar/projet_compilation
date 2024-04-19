void main() {
    int x = 0x1;
    int y = 0xFFFFF; // Overflow de ori

    int add = x + y;
    int sub = x - y;
    int mul = x * y;
    int div = x / y;

    print("x = ", x);
    print("y = ", y);

    print("add = ", add);
    print("sub = ", sub);
    print("mul = ", mul);
    print("div = ", div);
}
