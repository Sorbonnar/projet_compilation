int int1 = 20;
bool bool1 = false;

void main() {
    int a = 5;
    int b = int1;
    bool c = true;
    int i;
    
    int add = a + b;
    int sub = a - b;
    int mul = a * b;
    int div = b / a;
    int mod = b % a;

    int band = a & b;
    int bor = a | b;
    int bxor = a ^ b;

    bool notC = !c;
    int minA = -a;
    int notA = ~a;

    int SL = a << 2;
    int SR = b >> 2;

    print("Addition: !", add);
    print("\nSubtraction: ", sub);
    print("\nMultiplication: ", mul);
    print("\nDivision: ", div);
    print("\nModulo: ", mod);
    
    if (c) {
        print("\nTrue");
    }
    else {
        print("\nFalse");
    }
    
    if (a == b) {
        print("\na == b");
    }
    else if (a != b) {
        print("\na != b");
    }

    if (a < b) {
        print("\na < b");
    }
    else if (a > b) {
        print("\na > b");
    }

    if (a <= b) {
        print("\na <= b");
    }
    else if (a >= b) {
        print("\na >= b");
    }

    print("\nBAND: ", band);
    print("\nBOR: ", bor);
    print("\nBXOR: ", bxor);

    print("\n!C: ", notC);
    print("\n-A: ", minA);
    print("\n~A: ", notA);

    print("\nShift Left: ", SL);
    print("\nShift Right: ", SR);

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

    print("\nEnd of main");
    print("\n");
}
