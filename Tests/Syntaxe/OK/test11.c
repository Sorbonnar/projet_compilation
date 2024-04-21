// Expected output:
// a in while: 3
// a in while: 4
// a in while: 5
// a in while: 6
// a in while: 7
// i in for: 0
// i in for: 1
// i in for: 2
// i in for: 3
// i in for: 4
// b in do-while: 9
// b in do-while: 8
// b in do-while: 7
// b in do-while: 6
// b in do-while: 5
//

void main() {
    int a = 3;
    int b = 10;
    int i;

    while (a < 8) {
        print("\na in while: ", a);
        a = a + 1;
    }

    for (i = 0; i < 5; i = i + 1) {
        print("\ni in for: ", i);
    }

    do {
        b = b - 1;
        print("\nb in do-while: ", b);
    } while (b > 5);

    print("\n");
}
