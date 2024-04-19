void main() {
    int a = 3;
    int b = 10;
    int i;

    while (a < 8) {
        print("a in while: ", a);
        a = a + 1;
    }

    for (i = 0; i < 5; i = i + 1) {
        print("i in for: ", i);
    }

    do {
        b = b - 1;
        print("b in do-while: ", b);
    } while (b > 5);
}
