// Expected output:
// Sum: 15
// True
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

int int1 = 10;

void main() {
    int a = 5;
    int b = int1;
    int i;
    bool c = true;
    
    int sum = a + b;
    print("Sum: ", sum);
    
    if (c) {
        print("\nTrue");
    }
    else {
        print("\nFalse");
    }
    
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
}
